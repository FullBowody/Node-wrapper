#include "PluginWrap.hpp"

Napi::FunctionReference* PluginWrap::constructor = nullptr;

Napi::Value PluginWrap::NewInstance(Napi::Env env, PluginDescriptor descriptor)
{
    Napi::HandleScope scope(env);

    if (constructor == nullptr)
        return env.Null();

    PluginDescriptor* desc = new PluginDescriptor(descriptor);
    return constructor->New({Napi::External<PluginDescriptor>::New(env, desc)});
}

PluginWrap* PluginWrap::FromObject(Napi::Object obj)
{
    return Napi::ObjectWrap<PluginWrap>::Unwrap(obj);
}

Napi::Object PluginWrap::Init(Napi::Env env, Napi::Object exports)
{
    Napi::Function func = DefineClass(env, "Plugin", {
        InstanceMethod("getName", &PluginWrap::getName),
        InstanceMethod("getDescription", &PluginWrap::getDescription),
        InstanceMethod("getAuthor", &PluginWrap::getAuthor),
        InstanceMethod("getVersion", &PluginWrap::getVersion),
        InstanceMethod("getType", &PluginWrap::getType)
    });

    constructor = new Napi::FunctionReference();
    *constructor = Napi::Persistent(func);
    exports.Set("Plugin", func);

    return exports;
}

PluginWrap::PluginWrap(const Napi::CallbackInfo& info)
    : Napi::ObjectWrap<PluginWrap>(info)
{
    Napi::Env env = info.Env();
    Napi::HandleScope scope(env);

    Napi::External<PluginDescriptor> ext = info[0].As<Napi::External<PluginDescriptor>>();
    this->descriptor = ext.Data();
}

PluginWrap::~PluginWrap()
{
    delete this->descriptor;
}

Napi::Value PluginWrap::getName(const Napi::CallbackInfo& info)
{
    Napi::Env env = info.Env();
    std::string res = this->descriptor->getName();
    return Napi::String::New(env, res);
}

Napi::Value PluginWrap::getDescription(const Napi::CallbackInfo& info)
{
    Napi::Env env = info.Env();
    std::string res = this->descriptor->getDescription();
    return Napi::String::New(env, res);
}

Napi::Value PluginWrap::getAuthor(const Napi::CallbackInfo& info)
{
    Napi::Env env = info.Env();
    std::string res = this->descriptor->getAuthor();
    return Napi::String::New(env, res);
}

Napi::Value PluginWrap::getVersion(const Napi::CallbackInfo& info)
{
    Napi::Env env = info.Env();
    std::string res = this->descriptor->getVersion();
    return Napi::String::New(env, res);
}

Napi::Value PluginWrap::getType(const Napi::CallbackInfo& info)
{
    Napi::Env env = info.Env();

    std::string strType;
    switch (this->descriptor->getType())
    {
    case PluginType::CAMERA: strType = "Camera"; break;
    default: strType = "Unknown"; break;
    }

    return Napi::String::New(info.Env(), strType);
}

const PluginDescriptor& PluginWrap::getPluginDescriptor()
{
    return *this->descriptor;
}
