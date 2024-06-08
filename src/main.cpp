#include <napi.h>
#include "EngineLoader.hpp"
#include "EngineWrap.hpp"
#include "CameraWrap.hpp"
#include "PluginWrap.hpp"
#include "SceneWrap.hpp"
#include "PoseWrap.hpp"
#include "Vec3Wrap.hpp"
#include "QuaternionWrap.hpp"
#include "MarkerWrap.hpp"

EngineLoader* loader = nullptr;

#define register(x, y) exports.Set(Napi::String::New(env, x), Napi::Function::New(env, y))

Napi::Value CreateEngine(const Napi::CallbackInfo& info)
{
    Napi::Env env = info.Env();

    if (loader != nullptr)
    {
        std::cerr << "Error : Engine already created" << std::endl;
        return env.Null();
    }

    if (info.Length() > 0)
        loader = new EngineLoader(info[0].As<Napi::String>().Utf8Value());
    else loader = new EngineLoader();

    Engine* engine = loader->createEngine();
    if (engine == nullptr)
    {
        std::cerr << "Error : Engine creation failed" << std::endl;
        loader = nullptr;
        return env.Null();
    }
    return EngineWrap::NewInstance(env, engine);
}

Napi::Value DestroyEngine(const Napi::CallbackInfo& info)
{
    Napi::Env env = info.Env();
    if (loader == nullptr)
    {
        std::cerr << "Error : Engine not created" << std::endl;
        return Napi::Boolean::New(env, false);
    }

    Napi::Object wrapper = info[0].As<Napi::Object>();
    EngineWrap* engineWrap = Napi::ObjectWrap<EngineWrap>::Unwrap(wrapper);
    loader->destroyEngine(engineWrap->getEngine());
    return Napi::Boolean::New(env, true);
}

Napi::Object Register(Napi::Env env, Napi::Object exports)
{
    register("createEngine", CreateEngine);
    register("destroyEngine", DestroyEngine);

    EngineWrap::Init(env, exports);
    CameraWrap::Init(env, exports);
    PluginWrap::Init(env, exports);
    SceneWrap::Init(env, exports);
    PoseWrap::Init(env, exports);
    Vec3Wrap::Init(env, exports);
    QuaternionWrap::Init(env, exports);
    MarkerWrap::Init(env, exports);

    return exports;
}

NODE_API_MODULE(FullBowody, Register);
