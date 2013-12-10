
#include "ppapi/cpp/instance.h"
#include "ppapi/cpp/module.h"
#include "ppapi/cpp/var.h"

class HelloTutorialInstance : public pp::Instance {
 public:
  explicit HelloTutorialInstance(PP_Instance instance) :
      pp::Instance(instance) {}
  virtual ~HelloTutorialInstance() {}

  virtual void HandleMessage(const pp::Var& var_message) {
    /// DO STUFF!!!

    pp::Var var_reply("HelloTutorialInstance: Oh, someone send me a msg!");
    PostMessage(var_reply);

    pp::Var var_reply2("HelloTutorialInstance: The message was:" +
                       var_message.AsString());
    PostMessage(var_reply2);
  }
};

class HelloTutorialModule : public pp::Module {
 public:
  HelloTutorialModule() : pp::Module() {}
  virtual ~HelloTutorialModule() {}

  virtual pp::Instance* CreateInstance(PP_Instance instance) {
    return new HelloTutorialInstance(instance);
  }
};

namespace pp {
Module* CreateModule() {
  return new HelloTutorialModule();
}
}  // namespace pp
