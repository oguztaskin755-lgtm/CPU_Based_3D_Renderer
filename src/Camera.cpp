#include "Camera.h"

#include <cmath>


void Camera::UpdateDirection()
{
    const float cosYaw = cosf(math::toRadians(yaw));
    const float cosPitch = cosf(math::toRadians(pitch));

    const float sinYaw = sinf(math::toRadians(yaw));
    const float sinPitch = sinf(math::toRadians(pitch));

    forward.x = cosYaw * cosPitch;
    forward.y = sinPitch;
    forward.z = sinYaw * cosPitch;

    math::normalize(forward);

    right = math::returnNormalized(math::CrossProduct(forward, worldUp));
    up = math::returnNormalized(math::CrossProduct(right, forward));
}
