#include "PointLight.h"

#include <cmath>
#include <algorithm>
#include "../../rendering/Shader.h"
#include "../../core/ResourcesMgr.h"

PointLight::PointLight(const std::string& name, const glm::vec3& color, float intensity, const Attenuation& attenuation)
: PointLight(name, color, intensity, attenuation, 0)
{
	updateRange();
}

PointLight::PointLight(const std::string& name, const glm::vec3& color, float intensity, const Attenuation& attenuation, float range)
: Light(name, color, intensity), m_attenuation(attenuation), m_range(range)
{
	setShader(ResourcesMgr::shader("forward-point"));
}

void PointLight::updateRange()
{
	float minIntensity = 1 / (m_intensity * std::max(m_color.x, std::max(m_color.y, m_color.z)) * 256);
	float maxRange = 1000.0f;

	float range = maxRange;

	float c = m_attenuation.getConstant();
	float l = m_attenuation.getLinear();
	float e = m_attenuation.getExponent();
	if (m_intensity < minIntensity * c) 
		range = 0;
	else 
		if (e > 0) 
			range = (-l + (float)std::sqrt(l * l + 4.0 * e * (m_intensity / minIntensity - c))) / (2.0f * e);
		else if (l > 0)
			range = std::max(.0f, (m_intensity - minIntensity * c) / (minIntensity * l));

	m_range = std::min(range, maxRange);
}

PointLight::~PointLight()
{}

void PointLight::setIntensity(float intensity)
{
	m_intensity = std::max(.0f, intensity);
	updateRange();
}
