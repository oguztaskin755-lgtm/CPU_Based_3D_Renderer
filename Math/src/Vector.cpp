#include "../Includes/Vector.h"

#include <cmath>

namespace math
{
    float vec2::getMagnitude() const
    {
        return sqrtf(x * x + y * y);
    }


    float vec3::getMagnitude() const
    {
        return sqrtf(x * x + y * y + z * z);
    }

    float vec4::getMagnitude() const
    {
        return sqrtf(x * x + y * y + z * z + w * w);
    }

}
