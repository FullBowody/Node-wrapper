#include "Vec3Wrap.hpp"

Napi::Object Vec3Wrap::Init(Napi::Env env, Napi::Object exports)
{
    Napi::Function func = DefineClass(env, "Vec3", {
        InstanceAccessor("x", &Vec3Wrap::getX, &Vec3Wrap::setX),
        InstanceAccessor("y", &Vec3Wrap::getY, &Vec3Wrap::setY),
        InstanceAccessor("z", &Vec3Wrap::getZ, &Vec3Wrap::setZ)
    });

    constructor = new Napi::FunctionReference();
    *constructor = Napi::Persistent(func);
    exports.Set("Vec3", func);

    return exports;
}

Napi::Value Vec3Wrap::Create(const Napi::CallbackInfo& info)
{
    Napi::Env env = info.Env();
    return constructor->New({});
}

Napi::Value Vec3Wrap::Wrap(Napi::Env env, std::shared_ptr<glm::vec3> vec)
{
    Napi::Object obj = constructor->New({});
    Vec3Wrap* wrapper = Napi::ObjectWrap<Vec3Wrap>::Unwrap(obj);
    wrapper->vec = vec;
    return obj;
}

Vec3Wrap::Vec3Wrap(const Napi::CallbackInfo& info)
    : Napi::ObjectWrap<Vec3Wrap>(info)
{
    Napi::Env env = info.Env();

    if (info.Length() == 3)
    {
        float x = info[0].As<Napi::Number>().FloatValue();
        float y = info[1].As<Napi::Number>().FloatValue();
        float z = info[2].As<Napi::Number>().FloatValue();
        this->vec = std::make_shared<glm::vec3>(x, y, z);
    }
    else if (info.Length() == 0)
    {
        this->vec = std::make_shared<glm::vec3>();
    }
    else
    {
        Napi::TypeError::New(env, "Invalid arguments").ThrowAsJavaScriptException();
    }
}

Vec3Wrap::~Vec3Wrap()
{
    // nothing to delete : shared_ptr will take care of it
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

void Vec3Wrap::setX(const Napi::CallbackInfo& info, const Napi::Value& value)
{
    vec->x = value.As<Napi::Number>().FloatValue();
}

void Vec3Wrap::setY(const Napi::CallbackInfo& info, const Napi::Value& value)
{
    vec->y = value.As<Napi::Number>().FloatValue();
}

void Vec3Wrap::setZ(const Napi::CallbackInfo& info, const Napi::Value& value)
{
    vec->z = value.As<Napi::Number>().FloatValue();
}
