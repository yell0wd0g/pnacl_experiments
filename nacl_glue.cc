
#include "ppapi/cpp/instance.h"
#include "ppapi/cpp/module.h"
#include "ppapi/cpp/var.h"

#include "stitching.h"

class NaClGlueInstance : public pp::Instance {
 public:
  explicit NaClGlueInstance(PP_Instance instance) :
      pp::Instance(instance),
      stitching_() {

    std::string version_message("Initialised OpenCV version: ");
    version_message += stitching_.GetOpenCVVersion();
    PostMessage(pp::Var(version_message));
  }
  virtual ~NaClGlueInstance() {}

  virtual void HandleMessage(const pp::Var& var_message) {
    /// DO STUFF!!!
    pp::Var var_reply2("I got a message, content was:" +
                       var_message.AsString());
    PostMessage(var_reply2);
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
