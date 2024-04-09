#include <napi.h>
#include "EngineLoader.hpp"
#include "EngineWrap.hpp"
#include "CameraWrap.hpp"

#ifdef _WIN32
const std::string ENGINE_PATH = "./Engine.dll";
#else
const std::string ENGINE_PATH = "./Engine.so";
#endif

EngineLoader loader = EngineLoader(ENGINE_PATH);

#define register(x, y) exports.Set(Napi::String::New(env, x), Napi::Function::New(env, y))

Napi::Value CreateEngine(const Napi::CallbackInfo& info)
{
    Napi::Env env = info.Env();
    Engine* engine = loader.createEngine();
    return EngineWrap::NewInstance(env, engine);
}

Napi::Value DestroyEngine(const Napi::CallbackInfo& info)
{
    Napi::Env env = info.Env();
    Napi::Object wrapper = info[0].As<Napi::Object>();
    EngineWrap* engineWrap = Napi::ObjectWrap<EngineWrap>::Unwrap(wrapper);
    loader.destroyEngine(engineWrap->getEngine());
    return Napi::Boolean::New(env, true);
}

Napi::Object Register(Napi::Env env, Napi::Object exports)
{
    register("createEngine", CreateEngine);
    register("destroyEngine", DestroyEngine);

    EngineWrap::Init(env, exports);
    CameraWrap::Init(env, exports);

    return exports;
}

NODE_API_MODULE(FullBowody, Register);
