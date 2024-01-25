#include <napi.h>
#include "Engine/EngineLoader.hpp"

const std::string ENGINE_PATH = "./Engine.dll";

EngineLoader loader(ENGINE_PATH);
Engine* engine = nullptr;

void registerFunction(Napi::Object exports, Napi::Env env, std::string name, Napi::Value (*function)(const Napi::CallbackInfo&))
{
    exports.Set(Napi::String::New(env, name), Napi::Function::New(env, function));
}

Napi::Value Init(const Napi::CallbackInfo& info)
{
    Napi::Env env = info.Env();
    engine = loader.createEngine();
    return Napi::Boolean::New(env, true);
}

Napi::Value Run(const Napi::CallbackInfo& info)
{
    Napi::Env env = info.Env();
    engine->Run();
    return Napi::Boolean::New(env, true);
}

Napi::Value Shutdown(const Napi::CallbackInfo& info)
{
    Napi::Env env = info.Env();
    loader.destroyEngine(engine);
    return Napi::Boolean::New(env, true);
}

Napi::Object Init(Napi::Env env, Napi::Object exports)
{
    registerFunction(exports, env, "init", Init);
    registerFunction(exports, env, "run", Run);
    registerFunction(exports, env, "shutdown", Shutdown);

    return exports;
}

NODE_API_MODULE(FullBowody, Init);
