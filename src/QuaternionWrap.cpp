#include "QuaternionWrap.hpp"

Napi::FunctionReference* QuaternionWrap::constructor = nullptr;

Napi::Value QuaternionWrap::NewInstance(Napi::Env env, Quaternion* quat)
{
    Napi::HandleScope scope(env);

    if (constructor == nullptr)
        return env.Null();

    return constructor->New({Napi::External<Quaternion>::New(env, quat)});
}

QuaternionWrap* QuaternionWrap::FromObject(Napi::Object obj)
{
    return Napi::ObjectWrap<QuaternionWrap>::Unwrap(obj);
}

Napi::Object QuaternionWrap::Init(Napi::Env env, Napi::Object exports)
{
    Napi::Function func = DefineClass(env, "Quaternion", {
        InstanceMethod("setX", &QuaternionWrap::setX),
        InstanceMethod("setY", &QuaternionWrap::setY),
        InstanceMethod("setZ", &QuaternionWrap::setZ),
        InstanceMethod("setW", &QuaternionWrap::setW),
        InstanceMethod("getX", &QuaternionWrap::getX),
        InstanceMethod("getY", &QuaternionWrap::getY),
        InstanceMethod("getZ", &QuaternionWrap::getZ),
        InstanceMethod("getW", &QuaternionWrap::getW)
    });

    constructor = new Napi::FunctionReference();
    *constructor = Napi::Persistent(func);
    exports.Set("Quaternion", func);

    return exports;
}

QuaternionWrap::QuaternionWrap(const Napi::CallbackInfo& info)
    : Napi::ObjectWrap<QuaternionWrap>(info)
{
    Napi::Env env = info.Env();
    Napi::HandleScope scope(env);

    if (info.Length() == 4)
    {
        float x = info[0].As<Napi::Number>().FloatValue();
        float y = info[1].As<Napi::Number>().FloatValue();
        float z = info[2].As<Napi::Number>().FloatValue();
        float w = info[3].As<Napi::Number>().FloatValue();
        this->quat = new Quaternion(x, y, z, w);
    }
    else if (info.Length() == 1)
    {
        Napi::External<Quaternion> ext = info[0].As<Napi::External<Quaternion>>();
        this->quat = ext.Data();
    }
    else this->quat = new Quaternion();
}

QuaternionWrap::~QuaternionWrap()
{
    delete this->quat;
}

Napi::Value QuaternionWrap::setX(const Napi::CallbackInfo& info)
{
    Napi::Env env = info.Env();
    quat->x = info[0].As<Napi::Number>().FloatValue();
    return Napi::Boolean::New(env, true);
}

Napi::Value QuaternionWrap::setY(const Napi::CallbackInfo& info)
{
    Napi::Env env = info.Env();
    quat->y = info[0].As<Napi::Number>().FloatValue();
    return Napi::Boolean::New(env, true);
}

Napi::Value QuaternionWrap::setZ(const Napi::CallbackInfo& info)
{
    Napi::Env env = info.Env();
    quat->z = info[0].As<Napi::Number>().FloatValue();
    return Napi::Boolean::New(env, true);
}

Napi::Value QuaternionWrap::setW(const Napi::CallbackInfo& info)
{
    Napi::Env env = info.Env();
    quat->w = info[0].As<Napi::Number>().FloatValue();
    return Napi::Boolean::New(env, true);
}

Napi::Value QuaternionWrap::getX(const Napi::CallbackInfo& info)
{
    Napi::Env env = info.Env();
    return Napi::Number::New(env, quat->x);
}

Napi::Value QuaternionWrap::getY(const Napi::CallbackInfo& info)
{
    Napi::Env env = info.Env();
    return Napi::Number::New(env, quat->y);
}

Napi::Value QuaternionWrap::getZ(const Napi::CallbackInfo& info)
{
    Napi::Env env = info.Env();
    return Napi::Number::New(env, quat->z);
}

Napi::Value QuaternionWrap::getW(const Napi::CallbackInfo& info)
{
    Napi::Env env = info.Env();
    return Napi::Number::New(env, quat->w);
}

Quaternion* QuaternionWrap::getQuaternion()
{
    return quat;
}
