#include "EngineWrap.hpp"
#include "CameraWrap.hpp"

Napi::FunctionReference* EngineWrap::constructor = nullptr;

Napi::Value EngineWrap::NewInstance(Napi::Env env, Engine* engine)
{
    Napi::HandleScope scope(env);

    if (constructor == nullptr)
        return env.Null();

    return constructor->New({Napi::External<Engine>::New(env, engine)});
}

EngineWrap* EngineWrap::FromObject(Napi::Object obj)
{
    return Napi::ObjectWrap<EngineWrap>::Unwrap(obj);
}

Napi::Object EngineWrap::Init(Napi::Env env, Napi::Object exports)
{
    Napi::Function func = DefineClass(env, "Engine", {
        InstanceMethod("start", &EngineWrap::Start),
        InstanceMethod("update", &EngineWrap::Update),
        InstanceMethod("stop", &EngineWrap::Stop),
        InstanceMethod("createCamera", &EngineWrap::CreateCamera),
        InstanceMethod("destroyCamera", &EngineWrap::DestroyCamera)
    });

    constructor = new Napi::FunctionReference();
    *constructor = Napi::Persistent(func);
    exports.Set("Engine", func);

    return exports;
}

EngineWrap::EngineWrap(const Napi::CallbackInfo& info)
    : Napi::ObjectWrap<EngineWrap>(info)
{
    Napi::Env env = info.Env();
    Napi::HandleScope scope(env);

    Napi::External<Engine> ext = info[0].As<Napi::External<Engine>>();
    this->engine = ext.Data();
}

EngineWrap::~EngineWrap()
{
    
}

Napi::Value EngineWrap::Start(const Napi::CallbackInfo& info)
{
    Napi::Env env = info.Env();
    int res = this->engine->start();
    return Napi::Number::New(env, res);
}

Napi::Value EngineWrap::Update(const Napi::CallbackInfo& info)
{
    Napi::Env env = info.Env();
    float dt = info[0].As<Napi::Number>().FloatValue();
    int res = this->engine->update(dt);
    return Napi::Number::New(env, res);
}

Napi::Value EngineWrap::Stop(const Napi::CallbackInfo& info)
{
    Napi::Env env = info.Env();
    int res = this->engine->stop();
    return Napi::Number::New(env, res);
}

Napi::Value EngineWrap::CreateCamera(const Napi::CallbackInfo& info)
{
    Napi::Env env = info.Env();
    Camera* camera = this->engine->createCamera();
    return CameraWrap::NewInstance(env, camera);
}

Napi::Value EngineWrap::DestroyCamera(const Napi::CallbackInfo& info)
{
    Napi::Env env = info.Env();
    Napi::Object wrapper = info[0].As<Napi::Object>();
    CameraWrap* cameraWrap = CameraWrap::FromObject(wrapper);
    this->engine->destroyCamera(cameraWrap->getCamera());
    return Napi::Boolean::New(env, true);
}

Engine* EngineWrap::getEngine()
{
    return this->engine;
}
