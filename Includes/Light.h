#pragma once

#include <cstdint>

#include "../Math/Includes/Transform.h"



//TODO: Improve lighting, add blinn phong shading
//Currently lighting is just global light with flat shading which will get improved in the future
class Light
{
private:
    math::vec3 m_direction;

public:
    Light() = default;
    Light(const math::vec3& direction);

    uint32_t FlatShading(const math::vec3& normal, uint32_t color) const;
};
