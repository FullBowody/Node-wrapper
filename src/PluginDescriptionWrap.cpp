#include "PluginDescriptionWrap.hpp"
#include "PluginHandleWrap.hpp"

Napi::Object PluginDescriptionWrap::Init(Napi::Env env, Napi::Object exports)
{
    Napi::Function func = DefineClass(env, "PluginDescription", {
        InstanceMethod("getName", &PluginDescriptionWrap::getName),
        InstanceMethod("getDescription", &PluginDescriptionWrap::getDescription),
        InstanceMethod("getType", &PluginDescriptionWrap::getType),
        InstanceMethod("getAuthor", &PluginDescriptionWrap::getAuthor),
        InstanceMethod("getVersion", &PluginDescriptionWrap::getVersion),
        InstanceMethod("createHandle", &PluginDescriptionWrap::createHandle)
    });

    constructor = new Napi::FunctionReference();
    *constructor = Napi::Persistent(func);
    exports.Set("PluginDescription", func);
    env.SetInstanceData<Napi::FunctionReference>(constructor);

    return exports;
}

Napi::Value PluginDescriptionWrap::Create(const Napi::CallbackInfo& info)
{
    Napi::Env env = info.Env();
    return constructor->New({});
}

Napi::Value PluginDescriptionWrap::Wrap(Napi::Env env, std::shared_ptr<PluginDescription> pluginDescription)
{
    Napi::Object obj = constructor->New({});
    PluginDescriptionWrap* wrapper = Napi::ObjectWrap<PluginDescriptionWrap>::Unwrap(obj);
    wrapper->pluginDescription = pluginDescription;
    return obj;
}

PluginDescriptionWrap::PluginDescriptionWrap(const Napi::CallbackInfo& info)
    : Napi::ObjectWrap<PluginDescriptionWrap>(info)
{
    Napi::Env env = info.Env();
    // Napi::TypeError::New(env, "Use the engine.getPlugin method to create a PluginDescription object").ThrowAsJavaScriptException();
}

PluginDescriptionWrap::~PluginDescriptionWrap()
{
    // nothing to delete : shared_ptr will take care of it
}

Napi::Value PluginDescriptionWrap::getName(const Napi::CallbackInfo& info)
{
    return Napi::String::New(info.Env(), this->pluginDescription->getName());
}

Napi::Value PluginDescriptionWrap::getDescription(const Napi::CallbackInfo& info)
{
    return Napi::String::New(info.Env(), this->pluginDescription->getDescription());
}

Napi::Value PluginDescriptionWrap::getType(const Napi::CallbackInfo& info)
{
    std::string str;
    switch (this->pluginDescription->getType())
    {
        case PluginType::CAPTURE: str = "CAPTURE"; break;    
        default: str = "UNKNOWN"; break;
    }
    return Napi::String::New(info.Env(), str);
}

Napi::Value PluginDescriptionWrap::getAuthor(const Napi::CallbackInfo& info)
{
    return Napi::String::New(info.Env(), this->pluginDescription->getAuthor());
}

Napi::Value PluginDescriptionWrap::getVersion(const Napi::CallbackInfo& info)
{
    return Napi::String::New(info.Env(), this->pluginDescription->getVersion());
}

Napi::Value PluginDescriptionWrap::createHandle(const Napi::CallbackInfo& info)
{
    PluginHandle<Capture>* handle = this->pluginDescription->createHandle<Capture>();
    if (handle == nullptr)
        return info.Env().Null();
    return PluginHandleWrap::Wrap(info.Env(), handle);
}
