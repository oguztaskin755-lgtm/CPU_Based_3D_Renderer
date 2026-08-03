#include "Mesh.h"

#include <algorithm>

#include "../Math/Includes/Matrix.h"
#include "Light.h"
#include "Clipping.h"


Mesh::Mesh(const Mesh& other) noexcept
{
    m_vertices = other.m_vertices;
    m_indices = other.m_indices;

    m_normals = other.m_normals;
    m_normalIndices = other.m_normalIndices;

    m_texCoords = other.m_texCoords;
    m_texIndices = other.m_texIndices;

    m_modelMatrix = other.m_modelMatrix;
    m_viewMatrix = other.m_viewMatrix;
    m_projectionMatrix = other.m_projectionMatrix;

    m_texture = other.m_texture;

    //If you try to copy a mesh vectors wont have the same capacity, this can cause a huge heap allocation in RenderMesh when we push to std vectors
    ReserveVertices();
}

Mesh& Mesh::operator=(const Mesh& other) noexcept
{
    if (this == &other)
    {
        return *this;
    }

    m_vertices = other.m_vertices;
    m_indices = other.m_indices;

    m_normals = other.m_normals;
    m_normalIndices = other.m_normalIndices;

    m_texCoords = other.m_texCoords;
    m_texIndices = other.m_texIndices;

    m_modelMatrix = other.m_modelMatrix;
    m_viewMatrix = other.m_viewMatrix;
    m_projectionMatrix = other.m_projectionMatrix;

    m_texture = other.m_texture;

    //If you try to copy a mesh vectors wont have the same capacity, this can cause a huge heap allocation in RenderMesh when we push to std vectors
    ReserveVertices();

    return *this;
}

Mesh::Mesh(const std::vector<math::vec3> &vertices, const std::vector<unsigned int> &indices, const std::vector<math::vec3> &normals, const std::vector<unsigned int> &normalIndices, const std::vector<math::vec2> &texCoords, const std::vector<unsigned int> &textureIndices)
    :m_vertices(vertices), m_indices(indices), m_normals(normals), m_normalIndices(normalIndices), m_texCoords(texCoords), m_texIndices(textureIndices),
    m_modelMatrix(math::IdentityMat4()), m_viewMatrix(math::IdentityMat4()), m_projectionMatrix(math::IdentityMat4())
{
    ReserveVertices();
}

Mesh::Mesh(std::vector<math::vec3>&& vertices, std::vector<unsigned int>&& indices, std::vector<math::vec3>&& normals, std::vector<unsigned int>&& normalIndices, std::vector<math::vec2>&& texCoords, std::vector<unsigned int>&& textureIndices)
    :m_vertices(std::move(vertices)), m_indices(std::move(indices)), m_normals(std::move(normals)), m_normalIndices(std::move(normalIndices)), m_texCoords(std::move(texCoords)), m_texIndices(std::move(textureIndices)),
    m_modelMatrix(math::IdentityMat4()), m_viewMatrix(math::IdentityMat4()), m_projectionMatrix(math::IdentityMat4())
{
    ReserveVertices();
}


void Mesh::ReserveVertices()
{
    m_transformedVertices.reserve(m_vertices.size());
    m_trianglesToRender.reserve(m_indices.size() / 3);

    m_clipPolygon.vertices.reserve(3);
    m_clipPolygon.uvCoords.reserve(3);
}

void Mesh::AddTexture(const std::filesystem::path& fileName)
{
    LoadImageTexture(m_texture, fileName);
}


void Mesh::SetModelMatrix(const math::mat4& modelMatrix)
{
    m_modelMatrix = modelMatrix;
}

void Mesh::SetViewMatrix(const math::mat4& viewMatrix)
{
    m_viewMatrix = viewMatrix;
}

void Mesh::SetProjectionMatrix(const math::mat4& projectionMatrix)
{
    m_projectionMatrix = projectionMatrix;
}


void Mesh::RenderMesh(Draw& draw, bool backCull, unsigned int mode)
{
    const unsigned int indexCount = m_indices.size() / 3;

    m_transformedVertices.clear();
    m_trianglesToRender.clear();


    const math::mat4 viewModelMat = m_viewMatrix * m_modelMatrix;


    //Transform all the vertices into view space and save it into transformed vertices
    for (size_t i = 0; i < m_vertices.size(); i++)
    {
        math::vec4 vertex = math::toVec4(m_vertices[i]);
        vertex = viewModelMat * vertex;

        m_transformedVertices.push_back(vertex);
    }



    for (size_t i = 0; i < indexCount; i++)
    {
        std::array<math::vec4, 3> transformedVerticesTriangle;

        for (size_t j = 0; j < 3; j++)
        {
            unsigned int vertIndex = m_indices[i * 3 + j];
            transformedVerticesTriangle[j] = m_transformedVertices[vertIndex];
        }


        if (backCull)
        {
            //Backface culling
            //Get B-A and C-A vector (A = transformedVertices[0]) then get the normal vector
            math::vec3 vector1 = math::toVec3(transformedVerticesTriangle[1] - transformedVerticesTriangle[0]);
            math::vec3 vector2 = math::toVec3(transformedVerticesTriangle[2] - transformedVerticesTriangle[0]);

            //Dont normalize the normal vector since all we care about is the sign
            math::vec3 normalVector = math::CrossProduct(vector1, vector2);

            
            //Check angle between normal and camera
            //Normally its camerapos - vector but since camera is at origin in the view space we can simplify this
            math::vec3 cameraVector = math::toVec3(transformedVerticesTriangle[0]) * -1.0f;
            float dotProduct = math::DotProduct(normalVector, cameraVector);

            //If dot product is negative it means the normal vector is facing away from the camera which means we dont have to render
            if (dotProduct < 0)
            {
                continue;
            }
        }

        //Get the vertices to the clipping space
        for (size_t j = 0; j < 3; j++)
        {
            transformedVerticesTriangle[j] = m_projectionMatrix * transformedVerticesTriangle[j];
        }


        m_clipPolygon.vertices.clear();
        m_clipPolygon.uvCoords.clear();

        for (size_t j = 0; j < 3; j++)
        {
            m_clipPolygon.vertices.push_back(transformedVerticesTriangle[j]);
        }


        if (!m_texIndices.empty())
        {
            for (size_t j = 0; j < 3; j++)
            {
                unsigned int textIndex = m_texIndices[i * 3 + j];

                if (m_texIndices.size() > textIndex)
                {
                    m_clipPolygon.uvCoords.push_back(m_texCoords[textIndex]);
                }
            }
        }

        ClipPolygon(m_clipPolygon);

        if (m_clipPolygon.vertices.empty())
        {
            continue;
        }

        //Create triangles from n vertex polygons
        TrianglesFromPolygon(m_clipPolygon, m_trianglesToRender);
    }


    for (size_t i = 0; i < m_trianglesToRender.size(); i++)
    {
        for (size_t j = 0; j < 3; j++)
        {
            math::vec4& vertex = m_trianglesToRender[i].points[j];

            //Perspective divide, x and y should be normalized between [-1, 1] and z should be normalized between [0, 1]
            math::PerspectiveDivide(vertex);

            //Due to floating point precision errors sometimes x and y can be less than -1 or more than 1
            vertex.x = std::clamp(vertex.x, -1.0f, 1.0f);
            vertex.y = std::clamp(vertex.y, -1.0f, 1.0f);

            //Get the vertices into the screen space
            math::ViewportProjection(vertex, draw.getWindow().getWidth(), draw.getWindow().getHeight());
        }
    }



    //Draw all the triangles
    for (size_t i = 0; i < m_trianglesToRender.size(); i++)
    {
        if (mode == MODE_WIREFRAME_VERTEX)
        {
            for (size_t j = 0; j < 3; j++)
            {
                draw.DrawRectangle(m_trianglesToRender[i].points[j].x, m_trianglesToRender[i].points[j].y, 5, 5, 0xFF0000FF);
            }
        }

        if (mode == MODE_FILL || mode == MODE_FILL_WIREFRAME)
        {
            draw.FillTriangleColor
            (
                m_trianglesToRender[i].points[0],
                m_trianglesToRender[i].points[1],
                m_trianglesToRender[i].points[2],

                m_trianglesToRender[i].color
            );
        }

        if (mode == MODE_WIREFRAME_VERTEX || mode == MODE_WIREFRAME || mode == MODE_FILL_WIREFRAME || mode == MODE_WIREFRAME_TEXTURE)
        {
            draw.DrawTriangleWireframe(m_trianglesToRender[i].points[0], m_trianglesToRender[i].points[1], m_trianglesToRender[i].points[2], 0xDDFF00FF);
        }


        if (mode == MODE_FILL_TEXTURE || mode == MODE_WIREFRAME_TEXTURE)
        {
            if (!m_texture.textureData.empty())
            {
                draw.FillTriangleTexture
                (
                    m_trianglesToRender[i].points[0],
                    m_trianglesToRender[i].uvCoords[0],

                    m_trianglesToRender[i].points[1],
                    m_trianglesToRender[i].uvCoords[1],

                    m_trianglesToRender[i].points[2],
                    m_trianglesToRender[i].uvCoords[2],

                    m_texture
                );
            }
            else
            {
                draw.FillTriangleColor
                (
                m_trianglesToRender[i].points[0],
                m_trianglesToRender[i].points[1],
                m_trianglesToRender[i].points[2],

                    m_trianglesToRender[i].color
                );
            }
        }
    }
}