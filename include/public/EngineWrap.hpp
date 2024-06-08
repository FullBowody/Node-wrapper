#pragma once
#include <napi.h>
#include "Engine.hpp"

class EngineWrap: public Napi::ObjectWrap<EngineWrap>
{
private:
    static Napi::FunctionReference* constructor;
    Engine* engine;

public:
    static Napi::Value NewInstance(Napi::Env env, Engine* engine);
    static EngineWrap* FromObject(Napi::Object obj);
    static Napi::Object Init(Napi::Env env, Napi::Object exports);

    EngineWrap(const Napi::CallbackInfo& info);
    ~EngineWrap();

    Napi::Value Start(const Napi::CallbackInfo& info);
    Napi::Value Update(const Napi::CallbackInfo& info);
    Napi::Value Stop(const Napi::CallbackInfo& info);

    Napi::Value CreateCamera(const Napi::CallbackInfo& info);
    Napi::Value GetCamera(const Napi::CallbackInfo& info);
    Napi::Value GetCameras(const Napi::CallbackInfo& info);
    Napi::Value DestroyCamera(const Napi::CallbackInfo& info);

    Napi::Value GetPlugins(const Napi::CallbackInfo& info);

    Engine* getEngine();
};
