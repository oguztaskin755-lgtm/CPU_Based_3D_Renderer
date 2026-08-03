#pragma once

#include <array>
#include <cstddef>

#include "Vector.h"

namespace math
{

    struct mat4
    {
        std::array<std::array<float, 4>, 4> data{};

        constexpr mat4() = default;
        constexpr mat4(const std::array<std::array<float, 4>, 4>& value) :data(value) {}


        constexpr mat4 operator+ (const mat4& matrix) const;
        constexpr mat4 operator- (const mat4& matrix) const;
        constexpr mat4 operator* (float scalar) const;
        constexpr mat4 operator/ (float scalar) const;

        constexpr mat4& operator+= (const mat4& matrix);
        constexpr mat4& operator-= (const mat4& matrix);
        constexpr mat4& operator*= (float scalar);
        constexpr mat4& operator/= (float scalar);

        constexpr mat4 operator* (const mat4& matrix) const;
        constexpr mat4& operator*= (const mat4& matrix);

        constexpr vec4 operator* (const vec4& vector) const;
        constexpr mat4 Transpose() const;
    };



    constexpr mat4 IdentityMat4();

    //Use .inl file for constexpr functions since the body of functions is too long and makes the code less readable
#include "Matrix.inl"

}