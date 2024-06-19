#include "SceneWrap.hpp"
#include "MarkerWrap.hpp"
#include "PoseWrap.hpp"

Napi::FunctionReference* SceneWrap::constructor = nullptr;

Napi::Value SceneWrap::NewInstance(Napi::Env env, Scene* scene)
{
    Napi::HandleScope scope(env);

    if (constructor == nullptr)
        return env.Null();

    return constructor->New({Napi::External<Scene>::New(env, scene)});
}

SceneWrap* SceneWrap::FromObject(Napi::Object obj)
{
    return Napi::ObjectWrap<SceneWrap>::Unwrap(obj);
}

Napi::Object SceneWrap::Init(Napi::Env env, Napi::Object exports)
{
    Napi::Function func = DefineClass(env, "Scene", {
        InstanceMethod("on", &EventEmitter::On),
        InstanceMethod("createMarker", &SceneWrap::createMarker),
        InstanceMethod("destroyMarker", &SceneWrap::destroyMarker),
        InstanceMethod("findMarker", &SceneWrap::findMarker),
        InstanceMethod("getMarker", &SceneWrap::getMarker),
        InstanceMethod("getMarkers", &SceneWrap::getMarkers),
    });

    constructor = new Napi::FunctionReference();
    *constructor = Napi::Persistent(func);
    exports.Set("Scene", func);

    return exports;
}

SceneWrap::SceneWrap(const Napi::CallbackInfo& info)
    : Napi::ObjectWrap<SceneWrap>(info)
{
    Napi::Env env = info.Env();
    Napi::HandleScope scope(env);

    Napi::External<Scene> ext = info[0].As<Napi::External<Scene>>();
    this->scene = ext.Data();
}

SceneWrap::~SceneWrap()
{
}

std::string SceneWrap::toString(SceneEvent event)
{
    switch (event)
    {
        case MARKER_CREATED: return "markerCreated";
        case MARKER_DESTROYED: return "markerDestroyed";
        default: return "";
    }
}

Napi::Value SceneWrap::createMarker(const Napi::CallbackInfo& info)
{
    Napi::Env env = info.Env();
    Napi::HandleScope scope(env);

    int id = info[0].As<Napi::Number>().Int32Value();
    const Pose& pose = PoseWrap::FromObject(info[1].As<Napi::Object>())->getPose();

    Marker* marker = scene->createMarker(id, pose);
    if (marker == nullptr) return env.Null();

    Napi::Value markerWrap = MarkerWrap::NewInstance(env, marker);
    emitEvent(info, toString(MARKER_CREATED), markerWrap);
    return markerWrap;
}

Napi::Value SceneWrap::destroyMarker(const Napi::CallbackInfo& info)
{
    Napi::Env env = info.Env();
    Napi::HandleScope scope(env);

    int index = info[0].As<Napi::Number>().Int32Value();
    Napi::Value markerWrap = getMarker(info);
    scene->destroyMarker(index);

    emitEvent(info, toString(MARKER_DESTROYED), markerWrap);
    return Napi::Boolean::New(env, true);
}

Napi::Value SceneWrap::findMarker(const Napi::CallbackInfo& info)
{
    Napi::Env env = info.Env();
    Napi::HandleScope scope(env);

    int id = info[0].As<Napi::Number>().Int32Value();
    Marker* marker = scene->findMarker(id);
    if (marker == nullptr) return env.Null();

    return MarkerWrap::NewInstance(env, marker);
}

Napi::Value SceneWrap::getMarker(const Napi::CallbackInfo& info)
{
    Napi::Env env = info.Env();
    Napi::HandleScope scope(env);

    int index = info[0].As<Napi::Number>().Int32Value();
    Marker* marker = scene->getMarker(index);
    if (marker == nullptr) return env.Null();

    return MarkerWrap::NewInstance(env, marker);
}

Napi::Value SceneWrap::getMarkers(const Napi::CallbackInfo& info)
{
    Napi::Env env = info.Env();
    Napi::HandleScope scope(env);

    std::vector<Marker*> markers = scene->getMarkers();
    Napi::Array arr = Napi::Array::New(env, markers.size());
    for (int i = 0; i < markers.size(); i++)
        arr.Set(i, MarkerWrap::NewInstance(env, markers[i]));

    return arr;
}

const Scene& SceneWrap::getScene()
{
    return *scene;
}
