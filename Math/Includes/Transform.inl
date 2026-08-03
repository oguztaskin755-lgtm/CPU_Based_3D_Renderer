#pragma once



constexpr mat4 ScaleMat4(float scaleX, float scaleY, float scaleZ)
{
    mat4 scaleMatrix = math::IdentityMat4();

    scaleMatrix.data[0][0] = scaleX;
    scaleMatrix.data[1][1] = scaleY;
    scaleMatrix.data[2][2] = scaleZ;

    return scaleMatrix;
}



constexpr mat4 TranslateMat4(float dx, float dy, float dz)
{
    mat4 result = IdentityMat4();

    result.data[0][3] = dx;
    result.data[1][3] = dy;
    result.data[2][3] = dz;

    return result;
}


constexpr void PerspectiveDivide(vec4& vector)
{
    if (vector.w == 0.0f)
    {
        vector.w = 1.0f;
    }
    vector.x /= vector.w;
    vector.y /= vector.w;
    vector.z /= vector.w;
}

constexpr void ViewportProjection(vec4& vector, float width, float height)
{
    vector.x *= width * 0.5f;
    vector.y *= height * 0.5f;

    vector.x += width * 0.5f;
    vector.y += height * 0.5f;
}