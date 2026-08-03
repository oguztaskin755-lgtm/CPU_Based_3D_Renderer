#pragma once

#include "../Math/Includes/Transform.h"
#include "Mesh.h"
#include "Draw.h"

class GameObject
{
private:
    Mesh m_mesh;

public:
    math::vec3 m_scale = math::vec3(1.0f, 1.0f, 1.0f);
    math::vec3 m_rotation = math::vec3(0.0f, 0.0f, 0.0f);
    math::vec3 m_position = math::vec3(0.0f, 0.0f, 0.0f);

    GameObject(Mesh&& mesh);
    GameObject(Mesh&& mesh, const math::vec3& scale, const math::vec3& rotation, const math::vec3& position);

    GameObject(const Mesh& mesh);
    GameObject(const Mesh& mesh, const math::vec3& scale, const math::vec3& rotation, const math::vec3& position);

    void SetMeshMatrices(const math::mat4& viewMatrix, const math::mat4& projectionMatrix);
    void RenderObject(Draw& draw, bool backCull, unsigned int mode);
};
