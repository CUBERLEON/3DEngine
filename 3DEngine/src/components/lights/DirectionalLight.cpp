#include "DirectionalLight.h"

#include "../../core/Node.h"
#include "../../core/Transform.h"
#include "../../rendering/Shader.h"
#include "../../core/ResourcesMgr.h"
#include "../../core/Debug.h"

DirectionalLight::DirectionalLight(const std::string& name, const glm::vec3& color, float intensity)
: Light(name, color, intensity)
{
	setShader(ResourcesMgr::shader("forward-directional"));
}

DirectionalLight::~DirectionalLight()
{}

glm::vec3 DirectionalLight::getDirection()
{
	return glm::rotate(getNode()->getTransform()->getRealRotation(), glm::vec3(0, 0, -1));
}

glm::mat4 DirectionalLight::getShadowTransform()
{
	float k = 60;
	float size = k * 1;
	GLfloat near_plane = 1.0f, far_plane = 2*size;
	glm::mat4 lightProjection = glm::ortho(-size, size, -size, size, near_plane, far_plane);

	glm::mat4& lightTranslation = glm::translate(-(getNode()->getTransform()->getRealBack() * k * 1));
	glm::mat4& lightRotation = glm::transpose(glm::toMat4(getNode()->getTransform()->getRealRotation()));
	glm::mat4& viewTransform = lightRotation * lightTranslation;

	return lightProjection * viewTransform;
}
