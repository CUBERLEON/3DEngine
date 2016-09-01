#ifndef SPOT_LIGHT_H
#define SPOT_LIGHT_H

#include <memory>
#include <string>
#include <glm/glm.hpp>
#include "PointLight.h"

class SpotLight : public PointLight
{
public:
	SpotLight(const std::string& name, const glm::vec3& color, float intensity, const Attenuation& attenuation, float cutoff);
	SpotLight(std::shared_ptr<PointLight>& pointLight, float cutoff);
	~SpotLight();

	glm::vec3 getDirection();

	inline float getCutoff() const { return m_cutoff; }
	inline void setCutoff(float cutoff) { m_cutoff = cutoff; }
protected:
	float m_cutoff;

	SpotLight(const std::string& name, const glm::vec3& color, float intensity, const Attenuation& attenuation, float range, float cutoff);
private:
};

#endif // SPOT_LIGHT_H
