#include "Light.h"

#include <algorithm>


Light::Light(const math::vec3& direction)
    :m_direction(direction)
{

}

uint32_t Light::FlatShading(const math::vec3& normal, uint32_t color) const
{
    float brightness = -math::DotProduct(normal, m_direction);

    brightness = std::clamp(brightness, 0.0f, 1.0f);

    const uint32_t a = 0X000000FF & color;
    const uint32_t r = static_cast<float> (0xFF000000 & color) * brightness;
    const uint32_t g = static_cast<float> (0x00FF0000 & color) * brightness;
    const uint32_t b = static_cast<float> (0x0000FF00 & color) * brightness;

    uint32_t resultColor = a | (r & 0xFF000000) | (g & 0x00FF0000) | (b & 0x0000FF00);

    return resultColor;
}
