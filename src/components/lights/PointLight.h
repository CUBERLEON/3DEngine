#ifndef POINT_LIGHT_H
#define POINT_LIGHT_H

#include <string>
#include <glm/glm.hpp>
#include "../Light.h"
#include "../../rendering/Attenuation.h"

class PointLight : public Light
{
public:
    PointLight(const std::string& name, const glm::vec3& color, float intensity, const Attenuation& attenuation);
    ~PointLight();

    void setIntensity(float intensity) override;

    inline const Attenuation& getAttenuation() const { return m_attenuation; }
    inline void setAttenuation(const Attenuation& attenuation) { m_attenuation = attenuation; }

    inline float getRange() const { return m_range; }
protected:
    Attenuation m_attenuation;
    float m_range;

    PointLight(const std::string& name, const glm::vec3& color, float intensity, const Attenuation& attenuation, float range);
private:
    void updateRange();
};

#endif // POINT_LIGHT_H
