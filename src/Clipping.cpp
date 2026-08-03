#include "Clipping.h"

constexpr unsigned int NEAR_PLANE = 0;
constexpr unsigned int FAR_PLANE = 1;
constexpr unsigned int LEFT_PLANE = 2;
constexpr unsigned int RIGHT_PLANE = 3;
constexpr unsigned int BOTTOM_PLANE = 4;
constexpr unsigned int TOP_PLANE = 5;



float ClipAgainstPlane(const math::vec4& vertex, unsigned int plane)
{

    //In clip space we can use the inequalities to find if the plane is inside or outside and we can also get the Dot Product from the inequalities
    switch (plane)
    {
        case NEAR_PLANE:
            //Normally the inequality is z >= 0 but to make sure floating precision error doesnt happen resulting in the value becoming too small use z - (small value) >= 0
            return vertex.z - 0.00001f;

        case FAR_PLANE:
            return vertex.w - vertex.z;

        case LEFT_PLANE:
            return vertex.w + vertex.x;

        case RIGHT_PLANE:
            return vertex.w - vertex.x;

        case BOTTOM_PLANE:
            return vertex.w + vertex.y;

        case TOP_PLANE:
            return vertex.w - vertex.y;
    }

    return 0.0f;
}

void ClipPolygon(Polygon& polygon)
{
    std::vector<math::vec4> verticesInside;
    std::vector<math::vec2> uvInside;

    for (size_t i = 0; i < 6; i++)
    {
        if (polygon.vertices.empty())
        {
            return;
        }

        verticesInside.clear();
        uvInside.clear();

        //Use pointers to avoid copying
        const math::vec4* previousVertex = &polygon.vertices[polygon.vertices.size() - 1];
        float previousDot = ClipAgainstPlane(*previousVertex, i);

        const math::vec2* previousUV = &polygon.uvCoords[polygon.uvCoords.size() - 1];


        for (size_t j = 0; j < polygon.vertices.size(); j++)
        {
            const math::vec4& currentVertex = polygon.vertices[j];
            float currentDot = ClipAgainstPlane(currentVertex, i);

            const math::vec2& currentUV = polygon.uvCoords[j];

            //Check if it has been changed from inside to outside or outside to inside
            if (currentDot * previousDot < 0.0f)
            {
                //Find the point thats between previous and current vertex that lies in the plane using linear interpolation
                //I = Q1 + t(Q2 - Q1)
                float t = previousDot / (previousDot - currentDot);


                math::vec4 point = *previousVertex + (currentVertex - *previousVertex) * t;
                math::vec2 uvCoord = *previousUV + (currentUV - *previousUV) * t;

                verticesInside.push_back(point);
                uvInside.push_back(uvCoord);
            }

            //Check if the vertex is inside the plane
            if (currentDot >= 0.0f)
            {
                verticesInside.push_back(currentVertex);
                uvInside.push_back(currentUV);
            }

            previousVertex = &currentVertex;
            previousUV = &currentUV;
            previousDot = currentDot;
        }

        //Avoid copying a vector, std swap for vector swaps the pointer and size/capacity etc, saving a lot of time
        //Dont use std move since it can reset verticesInsides and uvInsides capacity to 0
        std::swap(polygon.vertices, verticesInside);
        std::swap(polygon.uvCoords, uvInside);
    }
}


