#include "Light.h"

#include "../core/Core.h"
#include "../rendering/Renderer.h"
#include "../rendering/Shader.h"
#include "../core/Debug.h"

Light::Light(const std::string& name, const glm::vec3& color, float intensity) : Component(name), m_color(color), m_intensity(intensity)
{}

Light::~Light()
{}

void Light::addedToSceneEvent()
{
    getCore()->getRenderingEngine()->addLight(shared_from_this());
}

void Light::removedFromSceneEvent()
{
    getCore()->getRenderingEngine()->removeLight(shared_from_this());
}
