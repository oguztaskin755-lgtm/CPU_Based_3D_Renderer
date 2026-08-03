#pragma once

#include <array>
#include <vector>
#include <cstdint>

#include "../Math/Includes/Transform.h"


struct Triangle
{
    std::array<math::vec4, 3> points;
    std::array<math::vec2, 3> uvCoords;
    uint32_t color = 0xCCCCCCFF;

    constexpr Triangle() = default;
};


struct Polygon
{
    std::vector<math::vec4> vertices;
    std::vector<math::vec2> uvCoords;

    Polygon() = default;
};


void TrianglesFromPolygon(const Polygon& polygon, std::vector<Triangle>& triangles);
bool isTopLeftEdge(const math::vec2& start, const math::vec2& end);