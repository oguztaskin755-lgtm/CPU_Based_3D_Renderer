#include "GameObject.h"

GameObject::GameObject(const Mesh& mesh)
    :m_mesh(mesh)
{

}

GameObject::GameObject(Mesh&& mesh)
    :m_mesh(std::move(mesh))
{

}

GameObject::GameObject(const Mesh& mesh, const math::vec3& scale, const math::vec3& rotation, const math::vec3& position)
    :m_mesh(mesh), m_scale(scale), m_rotation(rotation), m_position(position)
{

}

GameObject::GameObject(Mesh&& mesh, const math::vec3& scale, const math::vec3& rotation, const math::vec3& position)
    :m_mesh(std::move(mesh)), m_scale(scale), m_rotation(rotation), m_position(position)
{

}


void GameObject::SetMeshMatrices(const math::mat4& viewMatrix, const math::mat4& projectionMatrix)
{
    //Model matrix
    //Order is 1) scale 2) rotate 3) translate
    math::mat4 modelMat = math::ScaleMat4(m_scale.x, m_scale.y, m_scale.z);

    modelMat = math::RotateMatX4(m_rotation.x) * modelMat;
    modelMat = math::RotateMatY4(m_rotation.y) * modelMat;
    modelMat = math::RotateMatZ4(m_rotation.z) * modelMat;

    modelMat = math::TranslateMat4(m_position.x, m_position.y, m_position.z) * modelMat;

    m_mesh.SetModelMatrix(modelMat);
    m_mesh.SetViewMatrix(viewMatrix);
    m_mesh.SetProjectionMatrix(projectionMatrix);
}

void GameObject::RenderObject(Draw& draw, bool backCull, unsigned int mode)
{
    m_mesh.RenderMesh(draw, backCull, mode);
}
