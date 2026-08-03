#pragma once

#include "../Math/Includes/Transform.h"

struct Camera
{
    math::vec3 position;
    math::vec3 forward;
    math::vec3 up;
    math::vec3 right;
    math::vec3 worldUp;

    float yaw = -90.0f;
    float pitch = 0.0f;

    constexpr Camera() = default;
    void UpdateDirection();
};