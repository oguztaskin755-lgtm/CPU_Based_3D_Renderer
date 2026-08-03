#pragma once


namespace math
{
    struct vec2
    {
        float x = 0.0f;
        float y = 0.0f;

        constexpr vec2() = default;
        constexpr vec2(float xVal, float yVal) : x(xVal), y(yVal) {}


        //sqrt functions dont work with constexpr in C++ 17 so make it a normal function
        float getMagnitude() const;

        constexpr vec2 operator+ (const vec2& vector) const { return vec2(x + vector.x, y + vector.y); }
        constexpr vec2 operator- (const vec2& vector) const { return vec2(x - vector.x, y - vector.y); }
        constexpr vec2 operator* (float scalar) const { return vec2(x * scalar, y * scalar); }
        constexpr vec2 operator/ (float scalar) const { return vec2(x / scalar, y / scalar); }

        constexpr vec2& operator+= (const vec2& vector) { x += vector.x; y += vector.y; return *this; }
        constexpr vec2& operator-= (const vec2& vector) { x -= vector.x; y -= vector.y; return *this; }
        constexpr vec2& operator*= (float scalar) { x *= scalar; y *= scalar; return *this; }
        constexpr vec2& operator/= (float scalar) { x /= scalar; y /= scalar; return *this; }

    };

    struct vec3
    {
        float x = 0.0f;
        float y = 0.0f;
        float z = 0.0f;

        constexpr vec3() = default;
        constexpr vec3(float xVal, float yVal, float zVal) : x(xVal), y(yVal), z(zVal) {};


        float getMagnitude() const;

        constexpr vec3 operator+ (const vec3& vector) const { return vec3(x + vector.x, y + vector.y, z + vector.z); };
        constexpr vec3 operator- (const vec3& vector) const { return vec3(x - vector.x, y - vector.y, z - vector.z); };
        constexpr vec3 operator* (float scalar) const { return vec3(x * scalar, y * scalar, z * scalar); };
        constexpr vec3 operator/ (float scalar) const { return vec3(x / scalar, y / scalar, z / scalar); };

        constexpr vec3& operator+= (const vec3& vector) {x += vector.x; y += vector.y; z += vector.z; return *this; }
        constexpr vec3& operator-= (const vec3& vector) {x -= vector.x; y -= vector.y; z -= vector.z; return *this; }
        constexpr vec3& operator*= (float scalar) { x *= scalar; y *= scalar; z *= scalar; return *this; }
        constexpr vec3& operator/= (float scalar) { x /= scalar; y /= scalar; z /= scalar; return *this; }

    };

    struct vec4
    {
        float x = 0.0f;
        float y = 0.0f;
        float z = 0.0f;
        float w = 0.0f;

        constexpr vec4() = default;
        constexpr vec4(float xVal, float yVal, float zVal, float wVal) : x(xVal), y(yVal), z(zVal), w(wVal) {};


        float getMagnitude() const;

        constexpr vec4 operator+ (const vec4& vector) const { return vec4(x + vector.x, y + vector.y, z + vector.z, w + vector.w); }
        constexpr vec4 operator- (const vec4& vector) const { return vec4(x - vector.x, y - vector.y, z - vector.z, w - vector.w); }
        constexpr vec4 operator* (float scalar) const { return vec4(x * scalar, y * scalar, z * scalar, w * scalar); }
        constexpr vec4 operator/ (float scalar) const { return vec4(x / scalar, y / scalar, z / scalar, w / scalar); }

        constexpr vec4& operator+= (const vec4& vector) {x += vector.x; y += vector.y; z += vector.z; w += vector.w; return *this; }
        constexpr vec4& operator-= (const vec4& vector) {x -= vector.x; y -= vector.y; z -= vector.z; w -= vector.w; return *this; }
        constexpr vec4& operator*= (float scalar) { x *= scalar; y *= scalar; z *= scalar; w *= scalar; return *this; }
        constexpr vec4& operator/= (float scalar) {x /= scalar; y /= scalar; z /= scalar; w /= scalar; return *this; }
    };
}
