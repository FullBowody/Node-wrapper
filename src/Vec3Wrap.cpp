#include "Vec3Wrap.hpp"

Napi::FunctionReference* Vec3Wrap::constructor = nullptr;

Napi::Value Vec3Wrap::NewInstance(Napi::Env env, Vec3f* vec)
{
    Napi::HandleScope scope(env);

    if (constructor == nullptr)
        return env.Null();

    return constructor->New({Napi::External<Vec3f>::New(env, vec)});
}

Vec3Wrap* Vec3Wrap::FromObject(Napi::Object obj)
{
    return Napi::ObjectWrap<Vec3Wrap>::Unwrap(obj);
}

Napi::Object Vec3Wrap::Init(Napi::Env env, Napi::Object exports)
{
    Napi::Function func = DefineClass(env, "Vec3", {
        InstanceMethod("setX", &Vec3Wrap::setX),
        InstanceMethod("setY", &Vec3Wrap::setY),
        InstanceMethod("setZ", &Vec3Wrap::setZ),
        InstanceMethod("getX", &Vec3Wrap::getX),
        InstanceMethod("getY", &Vec3Wrap::getY),
        InstanceMethod("getZ", &Vec3Wrap::getZ)
    });

    constructor = new Napi::FunctionReference();
    *constructor = Napi::Persistent(func);
    exports.Set("Vec3", func);

    return exports;
}

Vec3Wrap::Vec3Wrap(const Napi::CallbackInfo& info)
    : Napi::ObjectWrap<Vec3Wrap>(info)
{
    Napi::Env env = info.Env();
    Napi::HandleScope scope(env);

    if (info.Length() == 3)
    {
        float x = info[0].As<Napi::Number>().FloatValue();
        float y = info[1].As<Napi::Number>().FloatValue();
        float z = info[2].As<Napi::Number>().FloatValue();
        this->vec = new Vec3f(x, y, z);
    }
    else if (info.Length() == 1)
    {
        Napi::External<Vec3f> ext = info[0].As<Napi::External<Vec3f>>();
        this->vec = ext.Data();
    }
    else this->vec = new Vec3f();

}

Vec3Wrap::~Vec3Wrap()
{
    delete this->vec;
}

Napi::Value Vec3Wrap::setX(const Napi::CallbackInfo& info)
{
    Napi::Env env = info.Env();
    vec->x = info[0].As<Napi::Number>().FloatValue();
    return Napi::Boolean::New(env, true);
}

Napi::Value Vec3Wrap::setY(const Napi::CallbackInfo& info)
{
    Napi::Env env = info.Env();
    vec->y = info[0].As<Napi::Number>().FloatValue();
    return Napi::Boolean::New(env, true);
}

Napi::Value Vec3Wrap::setZ(const Napi::CallbackInfo& info)
{
    Napi::Env env = info.Env();
    vec->z = info[0].As<Napi::Number>().FloatValue();
    return Napi::Boolean::New(env, true);
}

Napi::Value Vec3Wrap::getX(const Napi::CallbackInfo& info)
{
    Napi::Env env = info.Env();
    return Napi::Number::New(env, vec->x);
}

Napi::Value Vec3Wrap::getY(const Napi::CallbackInfo& info)
{
    Napi::Env env = info.Env();
    return Napi::Number::New(env, vec->y);
}

Napi::Value Vec3Wrap::getZ(const Napi::CallbackInfo& info)
{
    Napi::Env env = info.Env();
    return Napi::Number::New(env, vec->z);
}

Vec3f* Vec3Wrap::getVec3f()
{
    return vec;
}
