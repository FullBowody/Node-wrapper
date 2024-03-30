#include <napi.h>
#include "EngineLoader.hpp"

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
    engine->start();
    return Napi::Boolean::New(env, true);
}

Napi::Value Update(const Napi::CallbackInfo& info)
{
    Napi::Env env = info.Env();
    engine->update(0.1f);
    return Napi::Boolean::New(env, true);
}

Napi::Value Shutdown(const Napi::CallbackInfo& info)
{
    Napi::Env env = info.Env();
    engine->stop();
    loader.destroyEngine(engine);
    return Napi::Boolean::New(env, true);
}

Napi::Object Init(Napi::Env env, Napi::Object exports)
{
    registerFunction(exports, env, "init", Init);
    registerFunction(exports, env, "update", Update);
    registerFunction(exports, env, "shutdown", Shutdown);

    return exports;
}

NODE_API_MODULE(FullBowody, Init);
