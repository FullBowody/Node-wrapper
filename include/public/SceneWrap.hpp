#pragma once
#include <napi.h>
#include "Struct/Scene.hpp"
#include "EventEmitter.hpp"

class SceneWrap: public EventEmitter, public Napi::ObjectWrap<SceneWrap>
{
private:
    static Napi::FunctionReference* constructor;
    Scene* scene = nullptr;

    enum SceneEvent
    {
        MARKER_CREATED,
        MARKER_DESTROYED
    };
    std::string toString(SceneEvent event);

public:
    static Napi::Value NewInstance(Napi::Env env, Scene* scene);
    static SceneWrap* FromObject(Napi::Object obj);
    static Napi::Object Init(Napi::Env env, Napi::Object exports);

    SceneWrap(const Napi::CallbackInfo& info);
    ~SceneWrap();

    Napi::Value createMarker(const Napi::CallbackInfo& info);
    Napi::Value destroyMarker(const Napi::CallbackInfo& info);
    Napi::Value findMarker(const Napi::CallbackInfo& info);
    Napi::Value getMarker(const Napi::CallbackInfo& info);
    Napi::Value getMarkers(const Napi::CallbackInfo& info);

    Scene* getScene();
};
