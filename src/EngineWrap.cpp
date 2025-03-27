#include "EngineWrap.hpp"
#include "CameraWrap.hpp"
#include "PluginDescriptionWrap.hpp"

Napi::Object EngineWrap::Init(Napi::Env env, Napi::Object exports)
{
    Napi::Function func = DefineClass(env, "Engine", {
        InstanceMethod("update", &EngineWrap::update),
        InstanceMethod("createCamera", &EngineWrap::createCamera),
        InstanceMethod("destroyCamera", &EngineWrap::destroyCamera),
        InstanceMethod("getCameras", &EngineWrap::getCameras),
        InstanceMethod("getCamera", &EngineWrap::getCamera),
        InstanceMethod("getPlugins", &EngineWrap::getPlugins),
        InstanceMethod("getPlugin", &EngineWrap::getPlugin),
    });

    constructor = new Napi::FunctionReference();
    *constructor = Napi::Persistent(func);
    exports.Set("Engine", func);

    return exports;
}

Napi::Value EngineWrap::Create(const Napi::CallbackInfo& info)
{
    Napi::Env env = info.Env();
    return info.Length() == 0 ? constructor->New({}) : constructor->New({info[0]});
}

Napi::Value EngineWrap::Wrap(Napi::Env env, Engine* engine)
{
    Napi::Object obj = constructor->New({});
    EngineWrap* wrapper = Napi::ObjectWrap<EngineWrap>::Unwrap(obj);
    wrapper->engine = engine;
    return obj;
}

EngineWrap::EngineWrap(const Napi::CallbackInfo& info)
    : Napi::ObjectWrap<EngineWrap>(info)
{
    Napi::Env env = info.Env();
    // Napi::TypeError::New(env, "Use the [createEngine] method to instantiate an Engine").ThrowAsJavaScriptException();
}

EngineWrap::~EngineWrap()
{
    if (this->engine != nullptr)
    {
        delete this->engine;
        this->engine = nullptr;
    }
}

Napi::Value EngineWrap::update(const Napi::CallbackInfo& info)
{
    Napi::Env env = info.Env();
    float dt = info[0].As<Napi::Number>().FloatValue();
    FBError res = this->engine->update(dt);
    return Napi::Number::New(env, res);
}

Napi::Value EngineWrap::createCamera(const Napi::CallbackInfo& info)
{
    return CameraWrap::Wrap(info.Env(), this->engine->createCamera().lock());
}

Napi::Value EngineWrap::destroyCamera(const Napi::CallbackInfo& info)
{
    if (info.Length() < 1 || !info[0].IsObject())
    {
        Napi::TypeError::New(info.Env(), "Expected a Camera object").ThrowAsJavaScriptException();
        return info.Env().Undefined();
    }

    this->engine->destroyCamera(*CameraWrap::Unwrap(info[0].As<Napi::Object>())->getCamera());
    return info.Env().Undefined();
}

Napi::Value EngineWrap::getCameras(const Napi::CallbackInfo& info)
{
    auto& cameras = this->engine->getCameras();
    Napi::Array arr = Napi::Array::New(info.Env(), cameras.size());
    for (size_t i = 0; i < cameras.size(); i++)
    {
        arr.Set(i, CameraWrap::Wrap(info.Env(), cameras[i]));
    }
    return arr;
}

Napi::Value EngineWrap::getCamera(const Napi::CallbackInfo& info)
{
    if (info.Length() < 1 || !info[0].IsNumber())
    {
        Napi::TypeError::New(info.Env(), "Expected an index").ThrowAsJavaScriptException();
        return info.Env().Undefined();
    }

    size_t index = info[0].As<Napi::Number>().Uint32Value();
    auto& cameras = this->engine->getCameras();
    if (index >= this->engine->getCameras().size())
    {
        Napi::TypeError::New(info.Env(), "Index out of bounds").ThrowAsJavaScriptException();
        return info.Env().Undefined();
    }

    return CameraWrap::Wrap(info.Env(), this->engine->getCamera(index).lock());
}

Napi::Value EngineWrap::getPlugins(const Napi::CallbackInfo& info)
{
    const std::vector<std::shared_ptr<PluginDescription>>& plugins = this->engine->getPluginProvider().getPlugins();
    Napi::Array arr = Napi::Array::New(info.Env(), plugins.size());
    for (size_t i = 0; i < plugins.size(); i++)
    {
        arr.Set(i, PluginDescriptionWrap::Wrap(info.Env(), plugins[i]));
    }
    return arr;
}

Napi::Value EngineWrap::getPlugin(const Napi::CallbackInfo& info)
{
    if (info.Length() < 1 || !info[0].IsString())
    {
        Napi::TypeError::New(info.Env(), "Expected a plugin name").ThrowAsJavaScriptException();
        return info.Env().Undefined();
    }

    std::shared_ptr<PluginDescription> plugin = engine->getPluginProvider().getPlugin(info[0].As<Napi::String>().Utf8Value());
    if (!plugin) return info.Env().Null();
    return PluginDescriptionWrap::Wrap(info.Env(), plugin);
}
