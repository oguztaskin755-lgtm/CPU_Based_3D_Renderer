#pragma once

#include "Vector.h"
#include "Matrix.h"



namespace math
{
    constexpr float PI = 3.14159f;

    void normalize(vec2& vector);
    void normalize(vec3& vector);
    void normalize(vec4& vector);

    vec2 returnNormalized(const vec2& vector);
    vec3 returnNormalized(const vec3& vector);
    vec4 returnNormalized(const vec4& vector);

    constexpr vec2 toVec2(const vec3& vector) { return vec2(vector.x, vector.y); }
    constexpr vec2 toVec2(const vec4& vector) { return vec2(vector.x, vector.y); }

    constexpr vec3 toVec3(const vec2& vector) { return vec3(vector.x, vector.y, 0.0f); }
    constexpr vec3 toVec3(const vec4& vector) { return vec3(vector.x, vector.y, vector.z); }

    constexpr vec4 toVec4(const vec2& vector) { return vec4(vector.x, vector.y, 0.0f, 1.0f); }
    constexpr vec4 toVec4(const vec3& vector) { return vec4(vector.x, vector.y, vector.z, 1.0f); }


    constexpr float toDegrees(float radians) {  return radians * 180 / PI; }
    constexpr float toRadians(float degrees) {  return degrees * PI / 180; }


    constexpr float DotProduct(const vec2& vector1, const vec2& vector2) { return ( (vector1.x * vector2.x) + (vector1.y * vector2.y) ); }
    constexpr float DotProduct(const vec3& vector1, const vec3& vector2) {  return ( (vector1.x * vector2.x) + (vector1.y * vector2.y) + (vector1.z * vector2.z) ); }

    //Det of a 2x2 matrix is ad-bc
    constexpr float CrossProduct(const vec2& vector1, const vec2& vector2) { return vector1.x * vector2.y - vector1.y * vector2.x; }

    //Use the det of a 3x3 matrix
    constexpr vec3 CrossProduct(const vec3& vector1, const vec3& vector2)
    {
        //Use the determinate way to simplify the calculation
        return vec3((vector1.y * vector2.z - vector1.z * vector2.y), -(vector1.x * vector2.z - vector1.z * vector2.x), (vector1.x * vector2.y - vector1.y * vector2.x));
    }


    constexpr float EdgeFunction(const vec2& vector0, const vec2& vector1, const vec2& point) { return CrossProduct((vector1 - vector0), (point - vector0)); }

    constexpr mat4 ScaleMat4(float scaleX, float scaleY, float scaleZ);
    constexpr mat4 TranslateMat4(float dx, float dy, float dz);


    //Trigonometric functions dont work with constexpr in C++ 17 so make it a normal function
    mat4 RotateMatX4(float angle);
    mat4 RotateMatY4(float angle);
    mat4 RotateMatZ4(float angle);



    mat4 PerspectiveProjectionMat4(float fov, float aspectRatio, float near, float far);


    constexpr void PerspectiveDivide(vec4& vector);
    constexpr void ViewportProjection(vec4& vector, float width, float height);


    mat4 LookAtMatrix(const math::vec3& eye, const math::vec3& target, const math::vec3& up);


    vec3 BaryCentricCoordinate(const math::vec2& ver0, const math::vec2& ver1, const math::vec2& ver2, const math::vec2& point, float inverseArea);
    

    //For constexpr functions that have a long body
#include "Transform.inl"
}
