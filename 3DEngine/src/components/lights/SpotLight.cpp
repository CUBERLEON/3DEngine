#include "SpotLight.h"

#include "../../core/Node.h"
#include "../../core/Transform.h"
#include "../../rendering/Shader.h"
#include "../../core/ResourcesMgr.h"

SpotLight::SpotLight(const std::string& name, const glm::vec3& color, float intensity, const Attenuation& attenuation, float cutoff)
: PointLight(name, color, intensity, attenuation), m_cutoff(cutoff)
{
    setShader(ResourcesMgr::shader("forward-spot"));
}

SpotLight::SpotLight(std::shared_ptr<PointLight>& pointLight, float cutoff)
: SpotLight(pointLight->getName(), pointLight->getColor(), pointLight->getIntensity(), pointLight->getAttenuation(), pointLight->getRange(), cutoff)
{}

SpotLight::SpotLight(const std::string& name, const glm::vec3& color, float intensity, const Attenuation& attenuation, float range, float cutoff)
: PointLight(name, color, intensity, attenuation, range), m_cutoff(cutoff)
{
    setShader(ResourcesMgr::shader("forward-spot"));
}

SpotLight::~SpotLight()
{}

glm::vec3 SpotLight::getDirection()
{
    return glm::rotate(getNode()->getTransform()->getRealRotation(), glm::vec3(0, 0, -1));
}
