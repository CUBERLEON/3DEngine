#ifndef LIGHT_H
#define LIGHT_H

#include <memory>
#include <glm/glm.hpp>
#include "Component.h"

class Shader;

class Light : public Component, public std::enable_shared_from_this<Light>
{
    friend class Shader;
public:
    const std::shared_ptr<Shader>& getShader() const { return m_shader; }

    const glm::vec3& getColor() const { return m_color; }
    float getIntensity() const { return m_intensity; }

    void setColor(const glm::vec3& color) { m_color = color; }
    virtual void setIntensity(float intensity) { m_intensity = intensity; }
protected:
    Light(const std::string& name, const glm::vec3& color, float intensity);
    virtual ~Light();

    glm::vec3 m_color;
    float m_intensity;

    std::shared_ptr<Shader> m_shader;
    inline void setShader(const std::shared_ptr<Shader>& shader) { m_shader = shader; }

    virtual glm::mat4 getShadowTransform() { return glm::mat4(); }
private:
    void addedToSceneEvent() override;
    void removedFromSceneEvent() override;
};

#endif // LIGHT_H
