#include "Scene.h"

#include "Core.h"
#include "../rendering/Renderer.h"
#include "../rendering/Window.h"
#include "Node.h"
#include "ResourcesMgr.h"
#include "Debug.h"

Scene::Scene() : m_root(ResourcesMgr::node("root")), m_core(nullptr)
{}

Scene::~Scene()
{}

void Scene::setCore(Core* core)
{
    m_core = core;
}

void Scene::initBase()
{
    init();
}

void Scene::updateBase(float time)
{
    m_root->update(time);

    update(time);
}

void Scene::render(Renderer& renderingEngine)
{
    renderingEngine.render(m_root);
}

std::shared_ptr<Node>& Scene::getRoot()
{
    return m_root;
}

Window* Scene::getWindow()
{
    return m_core->getWindow();
}

const std::shared_ptr<Skybox>& Scene::getSkybox() const
{
    return m_core->getRenderingEngine()->getSkybox();
}

void Scene::setSkybox(const std::shared_ptr<Skybox>& skybox)
{
    m_core->getRenderingEngine()->setSkybox(skybox);
}
