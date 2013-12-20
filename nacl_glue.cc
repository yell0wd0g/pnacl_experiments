
#include "ppapi/cpp/instance.h"
#include "ppapi/cpp/module.h"
#include "ppapi/cpp/var.h"
#include "ppapi/cpp/var_array.h"

#include "stitching.h"

class NaClGlueInstance : public pp::Instance, public MessageDispatcher {
 public:
  explicit NaClGlueInstance(PP_Instance instance) :
      pp::Instance(instance),
      stitching_(2),
      next_data_index_(0) {

    std::string banner("Initialised OpenCV version: ");
    banner += stitching_.GetOpenCVVersion();
    bool res = stitching_.InitialiseOpenCV(320, 240);
    banner += (res ? " - OK" : (" - " + stitching_.last_error()));
    SendMessage(banner);

    stitching_.SetMessageHandler(this);
  }
  virtual ~NaClGlueInstance() {}

  virtual void HandleMessage(const pp::Var& var_message) {
    if (var_message.is_string()) {
      SendMessage(var_message.AsString());

      bool result = stitching_.CalculateHomography();
      SendMessage(result ? "Done, OK" : (" - " + stitching_.last_error()));
    } else if (var_message.is_int()) {
      SendMessage("I got an int");
    } else if (var_message.is_double()) {
      next_data_index_ = static_cast<int>(var_message.AsDouble());
      SendMessage("Got a double, which I take as index of the next data");
    } else if (var_message.is_array()) {

      if (next_data_index_>0 && next_data_index_<2) {
        pp::VarArray array(var_message.pp_var());
        stitching_.SetImageData(next_data_index_, 240, 320, array);

        SendMessage("I got an array, passed as an image");
      } else {
        SendMessage("The index was out of bounds.");
      }
    }
  }

  virtual void SendMessage(std::string msg) {
    PostMessage(pp::Var(msg));
  }

 private:
  Stitching stitching_;
  int next_data_index_;

};

class NaClGlueModule : public pp::Module {
 public:
  NaClGlueModule() : pp::Module() {}
  virtual ~NaClGlueModule() {}

  virtual pp::Instance* CreateInstance(PP_Instance instance) {
    return new NaClGlueInstance(instance);
  }
};

namespace pp {
Module* CreateModule() {
  return new NaClGlueModule();
}
}  // namespace pp
