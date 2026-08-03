#include "../Includes/Transform.h"
#include <cmath>

namespace math
{
    void normalize(vec2& vector)
    {
        vector = (vector / vector.getMagnitude());
    }

    void normalize(vec3& vector)
    {
        vector = (vector / vector.getMagnitude());
    }

    void normalize(vec4& vector)
    {
        vector = (vector / vector.getMagnitude());
    }


    vec2 returnNormalized(const vec2& vector)
    {
        return vector / vector.getMagnitude();
    }

    vec3 returnNormalized(const vec3& vector)
    {
        return vector / vector.getMagnitude();
    }

    vec4 returnNormalized(const vec4& vector)
    {
        return  vector / vector.getMagnitude();
    }


    mat4 RotateMatX4(float angle)
    {
        mat4 result = IdentityMat4();

        const float cosAngle = cosf(angle);
        const float sinAngle = sinf(angle);

        result.data[1][1] = cosAngle;
        result.data[1][2] = -sinAngle;

        result.data[2][1] = sinAngle;
        result.data[2][2] = cosAngle;

        return result;
    }

    mat4 RotateMatY4(float angle)
    {
        mat4 result = IdentityMat4();

        const float cosAngle = cosf(angle);
        const float sinAngle = sinf(angle);

        result.data[0][0] = cosAngle;
        result.data[0][2] = sinAngle;

        result.data[2][0] = -sinAngle;
        result.data[2][2] = cosAngle;

        return result;
    }

    mat4 RotateMatZ4(float angle)
    {
        mat4 result = IdentityMat4();

        const float cosAngle = cosf(angle);
        const float sinAngle = sinf(angle);

        result.data[0][0] = cosAngle;
        result.data[0][1] = -sinAngle;

        result.data[1][0] = sinAngle;
        result.data[1][1] = cosAngle;

        return result;
    }


    mat4 PerspectiveProjectionMat4(float fov, float aspectRatio, float near, float far)
    {
        mat4 result;

        const float fovScale =  1.0f / tanf(fov / 2);

        const float scaleZ = -far / (far - near);
        const float translateZ = -far * near / (far - near);

        result.data[0][0] = fovScale * aspectRatio;

        //Invert y axis since for sdl3 origin is at top left and y increases as it goes down
        result.data[1][1] = -fovScale;

        result.data[2][2] = scaleZ;
        result.data[2][3] = translateZ;

        //Store z before normalization in w of vec4, multiply it by -1 because of the right hand coordinate system
        result.data[3][2] = -1.0f;

        return result;
    }

    mat4 LookAtMatrix(const math::vec3& eye, const math::vec3& target, const math::vec3& up)
    {
        const vec3 forwardVector = returnNormalized(eye - target);
        const vec3 rightVector = returnNormalized(CrossProduct(up, forwardVector));
        const vec3 upVector = CrossProduct(forwardVector, rightVector);

        mat4 result = IdentityMat4();



        //This matrix is what we would get if we multiplied translation and rotation camera matrix
        //Normally we would need the inverse of the camera matrix to get view matrix but since camera matrix is orthogonal its inverse is equivalent to its transpose
        result.data[0][0] = rightVector.x;
        result.data[0][1] = rightVector.y;
        result.data[0][2] = rightVector.z;
        result.data[0][3] = -math::DotProduct(rightVector, eye);

        result.data[1][0] = upVector.x;
        result.data[1][1] = upVector.y;
        result.data[1][2] = upVector.z;
        result.data[1][3] = -math::DotProduct(upVector, eye);

        result.data[2][0] = forwardVector.x;
        result.data[2][1] = forwardVector.y;
        result.data[2][2] = forwardVector.z;
        result.data[2][3] = -math::DotProduct(forwardVector, eye);


        return result;
    }

    vec3 BaryCentricCoordinate(const math::vec2& ver0, const math::vec2& ver1, const math::vec2& ver2, const math::vec2& point, float inverseArea)
    {
        const float areaAlpha = math::CrossProduct((ver1 - ver2), (point - ver2));
        const float areaBeta = math::CrossProduct((ver0 - point), (ver2 - point));

        const float a = areaAlpha * inverseArea;
        const float b = areaBeta * inverseArea;
        const float c = 1.0f - a - b;


        return math::vec3(a, b, c);
    }
}
