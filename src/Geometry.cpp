#include "Geometry.h"

void TrianglesFromPolygon(const Polygon& polygon, std::vector<Triangle>& triangles)
{
    if (polygon.vertices.size() <= 2)
    {
        return;
    }

    triangles.reserve(triangles.size() + polygon.vertices.size() - 2);

    for (size_t i = 0; i < polygon.vertices.size() - 2; i++)
    {
        Triangle triangleFromPolygon;

        triangleFromPolygon.points[0] = polygon.vertices[0];
        triangleFromPolygon.points[1] = polygon.vertices[i + 1];
        triangleFromPolygon.points[2] = polygon.vertices[i + 2];

        triangleFromPolygon.uvCoords[0] = polygon.uvCoords[0];
        triangleFromPolygon.uvCoords[1] = polygon.uvCoords[i + 1];
        triangleFromPolygon.uvCoords[2] = polygon.uvCoords[i + 2];

        triangles.push_back(triangleFromPolygon);
    }
}

bool isTopLeftEdge(const math::vec2& start, const math::vec2& end)
{
    const math::vec2 edge = end - start;

    const bool isTopEdge = edge.y == 0 && edge.x < 0;
    const bool isLeftEdge = edge.y > 0;

    return isTopEdge || isLeftEdge;
}
