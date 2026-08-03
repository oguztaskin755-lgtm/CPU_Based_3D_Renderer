#include "Texture.h"

#include <algorithm>
#include <iostream>
#include <ostream>


#define STB_IMAGE_IMPLEMENTATION
#include "../Dependencies/STB Image/stb_image.h"


void InitSTBImage()
{
    stbi_set_flip_vertically_on_load(true);
}



uint32_t TextureColor(float depth0, const math::vec2& uv0, float depth1, const math::vec2& uv1, float depth2, const math::vec2& uv2, const math::vec2& point, const math::vec3& baryCentric, float inverseW, const Texture& texture)
{
    float interpolatedU = uv0.x / depth0 * baryCentric.x + uv1.x / depth1 * baryCentric.y + uv2.x / depth2 * baryCentric.z;
    float interpolatedV = uv0.y / depth0 * baryCentric.x + uv1.y / depth1 * baryCentric.y + uv2.y / depth2 * baryCentric.z;


    //Perspective correctness
    interpolatedU /= inverseW;
    interpolatedV /= inverseW;

    //Make sure floating point precision errors dont result in uv coords not being between [0, 1]
    interpolatedU = std::clamp(interpolatedU, 0.0f, 1.0f);
    interpolatedV = std::clamp(interpolatedV, 0.0f, 1.0f);


    auto textureX = static_cast<unsigned int> (interpolatedU * static_cast<float> (texture.textureWidth));
    auto textureY = static_cast<unsigned int> (interpolatedV * static_cast<float> (texture.textureHeight));

    textureX = std::min(textureX, static_cast<unsigned int> (texture.textureWidth) - 1);
    textureY = std::min(textureY, static_cast<unsigned int> (texture.textureHeight) - 1);


    return texture.textureData[texture.textureWidth * textureY + textureX];
}

bool LoadImageTexture(Texture& texture, const std::filesystem::path& filePath)
{
    int width, height;
    std::string fileString = filePath.string();

    unsigned char* data = stbi_load(fileString.c_str(), &width, &height, NULL, 4);

    if (data == nullptr)
    {
        std::cerr << "Could not load texture file: " << filePath << "\n";
        return false;
    }

    texture.textureWidth = width;
    texture.textureHeight = height;

    texture.textureData.reserve(width * height);

    for (size_t i = 0; i < width * height; i++)
    {
        //Our color follows RGBA format where each part is 8 bits
        uint32_t color = data[i * 4] << 24 | data[i * 4 + 1] << 16 | data[i * 4 + 2] << 8 | data[i * 4 + 3];
        texture.textureData.push_back(color);
    }

    stbi_image_free(data);

    return true;
}
