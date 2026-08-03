#pragma once

#include <vector>
#include <cstdint>
#include <filesystem>

#include "../Math/Includes/Transform.h"


struct Texture
{
    std::vector<uint32_t> textureData;
    int textureWidth = 0;
    int textureHeight = 0;

    Texture() = default;
};

void InitSTBImage();

uint32_t TextureColor(float depth0, const math::vec2& uv0, float depth1, const math::vec2& uv1, float depth2, const math::vec2& uv2, const math::vec2& point, const math::vec3& baryCentric, float inverseW, const Texture& texture);

bool LoadImageTexture(Texture& texture, const std::filesystem::path& filePath);