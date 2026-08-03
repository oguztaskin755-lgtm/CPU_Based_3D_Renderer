#pragma once

#include <vector>
#include <filesystem>

#include "../Math/Includes/Transform.h"

#include "Draw.h"
#include "Texture.h"
#include "Geometry.h"


constexpr unsigned int MODE_WIREFRAME_VERTEX = 0;
constexpr unsigned int MODE_WIREFRAME = 1;
constexpr unsigned int MODE_FILL = 2;
constexpr unsigned int MODE_FILL_WIREFRAME = 3;
constexpr unsigned int MODE_FILL_TEXTURE = 4;
constexpr unsigned int MODE_WIREFRAME_TEXTURE = 5;

constexpr bool BACKFACE_CULL_ENABLED = true;
constexpr bool BACKFACE_CULL_DISABLED = false;



//TODO: Start using normal vertices


class Mesh
{
private:
    std::vector<math::vec3> m_vertices;
    std::vector<unsigned int> m_indices;

    std::vector<math::vec3> m_normals;
    std::vector<unsigned int> m_normalIndices;

    std::vector<math::vec2> m_texCoords;
    std::vector<unsigned int> m_texIndices;

    Texture m_texture;

    std::vector<Triangle> m_trianglesToRender;
    std::vector<math::vec4> m_transformedVertices;

    math::mat4 m_modelMatrix;
    math::mat4 m_viewMatrix;
    math::mat4 m_projectionMatrix;

    Polygon m_clipPolygon;

    void ReserveVertices();

public:
    Mesh() = default;
    Mesh(const std::vector<math::vec3>& vertices, const std::vector<unsigned int>& indices, const std::vector<math::vec3>& normals, const std::vector<unsigned int>& normalIndices, const std::vector<math::vec2>& texCoords, const std::vector<unsigned int>& textureIndices);
    Mesh(std::vector<math::vec3>&& vertices, std::vector<unsigned int>&& indices, std::vector<math::vec3>&& normals, std::vector<unsigned int>&& normalIndices, std::vector<math::vec2>&& texCoords, std::vector<unsigned int>&& textureIndices);


    Mesh(Mesh&&) noexcept = default;
    Mesh& operator=(Mesh&&) noexcept = default;

    Mesh(const Mesh& other) noexcept;
    Mesh& operator=(const Mesh& other) noexcept;


    void AddTexture(const std::filesystem::path& fileName);
    void RenderMesh(Draw& draw, bool backCull, unsigned int mode);

    void SetModelMatrix(const math::mat4& modelMatrix);
    void SetViewMatrix(const math::mat4& viewMatrix);
    void SetProjectionMatrix(const math::mat4& projectionMatrix);
};
