#include "PluginHandleWrap.hpp"

Napi::Object PluginHandleWrap::Init(Napi::Env env, Napi::Object exports)
{
    Napi::Function func = DefineClass(env, "PluginHandle", {
        // Nothing, just a wrapper around the PluginHandle object
    });

    constructor = new Napi::FunctionReference();
    *constructor = Napi::Persistent(func);
    exports.Set("PluginHandle", func);

    return exports;
}

Napi::Value PluginHandleWrap::Create(const Napi::CallbackInfo& info)
{
    Napi::Env env = info.Env();
    return constructor->New({});
}

Napi::Value PluginHandleWrap::Wrap(Napi::Env env, PluginHandle<Capture>* pluginHandle)
{
    Napi::Object obj = constructor->New({});
    PluginHandleWrap* wrapper = Napi::ObjectWrap<PluginHandleWrap>::Unwrap(obj);
    wrapper->pluginHandle = pluginHandle;
    return obj;
}

PluginHandleWrap::PluginHandleWrap(const Napi::CallbackInfo& info)
    : Napi::ObjectWrap<PluginHandleWrap>(info)
{
    Napi::Env env = info.Env();
    // Napi::TypeError::New(env, "Use the engine.getPlugin method to create a PluginHandle object").ThrowAsJavaScriptException();
}

PluginHandleWrap::~PluginHandleWrap()
{
    // nothing to delete : camera should take care of it
}
