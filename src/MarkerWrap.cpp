#include "MarkerWrap.hpp"
#include "PoseWrap.hpp"

Napi::FunctionReference* MarkerWrap::constructor = nullptr;

Napi::Value MarkerWrap::NewInstance(Napi::Env env, Marker* marker)
{
    Napi::HandleScope scope(env);

    if (constructor == nullptr)
        return env.Null();

    return constructor->New({Napi::External<Marker>::New(env, marker)});
}

MarkerWrap* MarkerWrap::FromObject(Napi::Object obj)
{
    return Napi::ObjectWrap<MarkerWrap>::Unwrap(obj);
}

Napi::Object MarkerWrap::Init(Napi::Env env, Napi::Object exports)
{
    Napi::Function func = DefineClass(env, "Marker", {
        InstanceMethod("on", &EventEmitter::on),
        InstanceMethod("setId", &MarkerWrap::setId),
        InstanceMethod("setPose", &MarkerWrap::setPose),
        InstanceMethod("getId", &MarkerWrap::getId),
        InstanceMethod("getPose", &MarkerWrap::getPose)
    });

    constructor = new Napi::FunctionReference();
    *constructor = Napi::Persistent(func);
    exports.Set("Marker", func);

    return exports;
}

MarkerWrap::MarkerWrap(const Napi::CallbackInfo& info)
    : Napi::ObjectWrap<MarkerWrap>(info)
{
    Napi::Env env = info.Env();
    Napi::HandleScope scope(env);

    if (info.Length() == 2)
    {
        int id = info[0].As<Napi::Number>().Int32Value();
        Pose* pose = PoseWrap::FromObject(info[1].As<Napi::Object>())->getPose();
        this->marker = new Marker(id, *pose);
    }
    else if (info.Length() == 1)
    {
        Napi::External<Marker> ext = info[0].As<Napi::External<Marker>>();
        this->marker = ext.Data();
    }
    else this->marker = new Marker();
}

MarkerWrap::~MarkerWrap()
{
}

std::string MarkerWrap::toString(MarkerEvent event)
{
    switch (event)
    {
    case ID_UPDATED: return "idUpdated";
    case POSE_UPDATED: return "poseUpdated";
    default: return "";
    }
}

Napi::Value MarkerWrap::setId(const Napi::CallbackInfo& info)
{
    Napi::Env env = info.Env();
    marker->setId(info[0].As<Napi::Number>().Int32Value());
    emitEvent(info, toString(ID_UPDATED), Napi::Number::New(env, marker->getId()));
    return Napi::Boolean::New(env, true);
}

Napi::Value MarkerWrap::setPose(const Napi::CallbackInfo& info)
{
    Napi::Env env = info.Env();
    Pose* pose = PoseWrap::FromObject(info[0].As<Napi::Object>())->getPose();
    if (pose == nullptr) return Napi::Boolean::New(env, false);
    marker->setPose(*pose);
    emitEvent(info, toString(POSE_UPDATED), PoseWrap::NewInstance(env, pose));
    return Napi::Boolean::New(env, true);
}

Napi::Value MarkerWrap::getId(const Napi::CallbackInfo& info)
{
    Napi::Env env = info.Env();
    return Napi::Number::New(env, marker->getId());
}

Napi::Value MarkerWrap::getPose(const Napi::CallbackInfo& info)
{
    Napi::Env env = info.Env();
    return PoseWrap::NewInstance(env, new Pose(marker->getPose()));
}

Marker* MarkerWrap::getMarker()
{
    return marker;
}
