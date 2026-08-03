#pragma once



constexpr mat4 mat4::operator+ (const mat4& matrix) const
{
    mat4 result;

    for (size_t i = 0; i < 4; i++)
    {
        for (size_t j = 0; j < 4; j++)
        {
            result.data[i][j] = data[i][j] + matrix.data[i][j];
        }
    }

    return result;
}

constexpr mat4 mat4::operator- (const mat4& matrix) const
{
    mat4 result;

    for (size_t i = 0; i < 4; i++)
    {
        for (size_t j = 0; j < 4; j++)
        {
            result.data[i][j] = data[i][j] - matrix.data[i][j];
        }
    }

    return result;
}

constexpr mat4 mat4::operator* (float scalar) const
{
    mat4 result;

    for (size_t i = 0; i < 4; i++)
    {
        for (size_t j = 0; j < 4; j++)
        {
            result.data[i][j] = data[i][j] * scalar;
        }
    }

    return result;
}

constexpr mat4 mat4::operator/ (float scalar) const
{
    mat4 result;

    for (size_t i = 0; i < 4; i++)
    {
        for (size_t j = 0; j < 4; j++)
        {
            result.data[i][j] = data[i][j] / scalar;
        }
    }

    return result;
}




constexpr mat4& mat4::operator+= (const mat4& matrix)
{
    for (size_t i = 0; i < 4; i++)
    {
        for (size_t j = 0; j < 4; j++)
        {
            data[i][j] += matrix.data[i][j];
        }
    }

    return *this;
}


constexpr mat4& mat4::operator-= (const mat4& matrix)
{
    for (size_t i = 0; i < 4; i++)
    {
        for (size_t j = 0; j < 4; j++)
        {
            data[i][j] -= matrix.data[i][j];
        }
    }

    return *this;
}


constexpr mat4& mat4::operator*= (float scalar)
{
    for (size_t i = 0; i < 4; i++)
    {
        for (size_t j = 0; j < 4; j++)
        {
            data[i][j] *= scalar;
        }
    }

    return *this;
}

constexpr mat4& mat4::operator/= (float scalar)
{
    for (size_t i = 0; i < 4; i++)
    {
        for (size_t j = 0; j < 4; j++)
        {
            data[i][j] /= scalar;
        }
    }

    return *this;
}


constexpr mat4 mat4::operator* (const mat4& matrix) const
{
    mat4 result;

    for (size_t i = 0; i < 4; i++)
    {
        for (size_t j = 0; j < 4; j++)
        {
            for (size_t k = 0; k < 4; k++)
            {
                result.data[j][i] += data[j][k] * matrix.data[k][i];
            }
        }
    }

    return result;
}



constexpr mat4& mat4::operator*= (const mat4& matrix)
{
    mat4 result;

    for (size_t i = 0; i < 4; i++)
    {
        for (size_t j = 0; j < 4; j++)
        {
            for (size_t k = 0; k < 4; k++)
            {
                result.data[j][i] += data[j][k] * matrix.data[k][i];
            }
        }
    }

    data = result.data;

    return *this;
}


constexpr vec4 mat4::operator* (const vec4& vector) const
{
    std::array<float, 4> result = {};
    const std::array<float, 4> vectorData = { vector.x, vector.y, vector.z, vector.w };

    for (size_t i = 0; i < 4; i++)
    {
        for (size_t j = 0; j < 4; j++)
        {
            result[i] += data[i][j] * vectorData[j];
        }
    }

    return vec4(result[0], result[1], result[2], result[3]);
}



constexpr mat4 IdentityMat4()
{
    return mat4({{
    {1.0f, 0.0f, 0.0f, 0.0f},
    {0.0f, 1.0f, 0.0f, 0.0f},
    {0.0f, 0.0f, 1.0f, 0.0f},
    {0.0f, 0.0f, 0.0f, 1.0f}
}});
}

constexpr mat4 mat4::Transpose() const
{
    mat4 result;

    for (size_t i = 0; i < 4; i++)
    {
        for (size_t j = 0; j < 4; j++)
        {
            result.data[i][j] = data[j][i];
        }
    }

    return result;
}
