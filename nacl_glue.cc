
#include "ppapi/cpp/instance.h"
#include "ppapi/cpp/module.h"
#include "ppapi/cpp/var.h"

#include "stitching.h"

class NaClGlueInstance : public pp::Instance {
 public:
  explicit NaClGlueInstance(PP_Instance instance) :
      pp::Instance(instance),
      stitching_(2) {

    std::string banner("Initialised OpenCV version: ");
    banner += stitching_.GetOpenCVVersion();

    bool res = stitching_.InitialiseOpenCV(320, 240);
    banner += (res ? " - OK" : (" - " + stitching_.last_error()));

    PostMessage(pp::Var(banner));
  }
  virtual ~NaClGlueInstance() {}

  virtual void HandleMessage(const pp::Var& var_message) {
    pp::Var var_reply2("I got this message: " +
                       var_message.AsString() +
                       " I'm going to calculate the homography.");
    PostMessage(var_reply2);

    bool result = stitching_.CalculateHomography();
    PostMessage(result ? "Done, OK" : (" - " + stitching_.last_error()));
  }

 private:
  Stitching stitching_;
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
