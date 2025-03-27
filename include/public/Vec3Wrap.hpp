#pragma once
#include <napi.h>
#include <memory>
#include <glm/glm.hpp>

class Vec3Wrap: public Napi::ObjectWrap<Vec3Wrap>
{
public:
    static Napi::Object Init(Napi::Env env, Napi::Object exports);
    static Napi::Value Create(const Napi::CallbackInfo& info);
    static Napi::Value Wrap(Napi::Env env, std::shared_ptr<glm::vec3> vec);

    Vec3Wrap(const Napi::CallbackInfo& info);
    ~Vec3Wrap();

    Napi::Value getX(const Napi::CallbackInfo& info);
    Napi::Value getY(const Napi::CallbackInfo& info);
    Napi::Value getZ(const Napi::CallbackInfo& info);
    void setX(const Napi::CallbackInfo& info, const Napi::Value& value);
    void setY(const Napi::CallbackInfo& info, const Napi::Value& value);
    void setZ(const Napi::CallbackInfo& info, const Napi::Value& value);

    const std::shared_ptr<glm::vec3>& getVec3() { return vec; }

private:
    inline static Napi::FunctionReference* constructor = nullptr;
    std::shared_ptr<glm::vec3> vec;
};
