#include "PoseWrap.hpp"
#include "Vec3Wrap.hpp"
#include "QuaternionWrap.hpp"

Napi::FunctionReference* PoseWrap::constructor = nullptr;

Napi::Value PoseWrap::NewInstance(Napi::Env env, Pose* pose)
{
    Napi::HandleScope scope(env);

    if (constructor == nullptr)
        return env.Null();

    return constructor->New({Napi::External<Pose>::New(env, pose)});
}

PoseWrap* PoseWrap::FromObject(Napi::Object obj)
{
    return Napi::ObjectWrap<PoseWrap>::Unwrap(obj);
}

Napi::Object PoseWrap::Init(Napi::Env env, Napi::Object exports)
{
    Napi::Function func = DefineClass(env, "Pose", {
        InstanceMethod("setPosition", &PoseWrap::setPosition),
        InstanceMethod("getPosition", &PoseWrap::getPosition),
        InstanceMethod("setRotation", &PoseWrap::setRotation),
        InstanceMethod("getRotation", &PoseWrap::getRotation)
    });

    constructor = new Napi::FunctionReference();
    *constructor = Napi::Persistent(func);
    exports.Set("Pose", func);

    return exports;
}

PoseWrap::PoseWrap(const Napi::CallbackInfo& info)
    : Napi::ObjectWrap<PoseWrap>(info)
{
    Napi::Env env = info.Env();
    Napi::HandleScope scope(env);

    if (info.Length() == 2)
    {
        const Vec3f& vec = Vec3Wrap::FromObject(info[0].As<Napi::Object>())->getVec3f();
        const Quaternion& quat = QuaternionWrap::FromObject(info[1].As<Napi::Object>())->getQuaternion();
        this->pose = new Pose(vec, quat);
    }
    else if (info.Length() == 1)
    {
        Napi::External<Pose> ext = info[0].As<Napi::External<Pose>>();
        this->pose = ext.Data();
    }
    else this->pose = new Pose();
}

PoseWrap::~PoseWrap()
{
    delete pose;
}

Napi::Value PoseWrap::setPosition(const Napi::CallbackInfo& info)
{
    Napi::Env env = info.Env();
    Napi::HandleScope scope(env);

    const Vec3f& vec = Vec3Wrap::FromObject(info[0].As<Napi::Object>())->getVec3f();
    pose->setPosition(vec);

    return Napi::Boolean::New(env, true);
}

Napi::Value PoseWrap::getPosition(const Napi::CallbackInfo& info)
{
    Napi::Env env = info.Env();
    return Vec3Wrap::NewInstance(env, new Vec3f(pose->getPosition()));
}

Napi::Value PoseWrap::setRotation(const Napi::CallbackInfo& info)
{
    Napi::Env env = info.Env();
    Napi::HandleScope scope(env);

    const Quaternion& quat = QuaternionWrap::FromObject(info[0].As<Napi::Object>())->getQuaternion();
    pose->setRotation(quat);

    return Napi::Boolean::New(env, true);
}

Napi::Value PoseWrap::getRotation(const Napi::CallbackInfo& info)
{
    Napi::Env env = info.Env();
    return QuaternionWrap::NewInstance(env, new Quaternion(pose->getRotation()));
}

const Pose& PoseWrap::getPose()
{
    return *pose;
}
