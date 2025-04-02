#pragma once
#include <napi.h>
#include <memory>
#include "Engine.hpp"

class EngineWrap: public Napi::ObjectWrap<EngineWrap>
{
public:
    static Napi::Object Init(Napi::Env env, Napi::Object exports);
    static Napi::Value Create(const Napi::CallbackInfo& info);
    static Napi::Value Wrap(Napi::Env env, Engine* engine);

    EngineWrap(const Napi::CallbackInfo& info);
    ~EngineWrap();

    Napi::Value start(const Napi::CallbackInfo& info);
    Napi::Value stop(const Napi::CallbackInfo& info);

    Napi::Value createCamera(const Napi::CallbackInfo& info);
    Napi::Value destroyCamera(const Napi::CallbackInfo& info);
    Napi::Value getCameras(const Napi::CallbackInfo& info);
    Napi::Value getCamera(const Napi::CallbackInfo& info);

    Napi::Value getPlugins(const Napi::CallbackInfo& info);
    Napi::Value getPlugin(const Napi::CallbackInfo& info);

    Engine* getEngine() { return engine; }
    
private:
    inline static Napi::FunctionReference* constructor = nullptr;
    Engine* engine;
};
