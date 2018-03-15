#include "Entity.h"

#include "../core/Node.h"
#include "../core/Frustum.h"
#include "../core/AABox.h"
#include "../core/Transform.h"
#include "Camera.h"
#include "../rendering/Mesh.h"
#include "../rendering/Shader.h"
#include "../rendering/Renderer.h"
#include "../core/Debug.h"

Entity::Entity(const std::string& name) : Component(name)
{}

Entity::~Entity()
{}

std::shared_ptr<Entity> Entity::addMesh(const std::shared_ptr<Mesh>& mesh)
{
    m_meshes.push_back(mesh);
    return shared_from_this();
}

std::shared_ptr<Entity> Entity::addMeshes(const std::vector< std::shared_ptr<Mesh> >& meshes)
{
    for (size_t i = 0; i < meshes.size(); ++i)
        m_meshes.push_back(meshes[i]);
    return shared_from_this();
}

void Entity::render(std::shared_ptr<Shader>& shader, Renderer& renderingEngine)
{
    shader->updateTransform(*getNode()->getTransform(), renderingEngine);

    for (size_t i = 0; i < m_meshes.size(); ++i)
    {
        if (shader->getName() == "shadow-map" || 
            renderingEngine.getActiveCamera()->getFrustum()->isBoxInside(m_meshes[i]->getBounds()->getTransformed(getNode()->getTransform()->getRealModelTransform())))
        {
            m_meshes[i]->draw(shader);
        }
    }
}