#ifndef MATERIAL_H
#define MATERIAL_H

#include <memory>
#include <glm/glm.hpp>

class Texture;
class ResourcesMgr;

class Material
{
    friend class ResourcesMgr;
public:
    ~Material();

    Material* setDiffuseTexture(const std::shared_ptr<Texture>& value) { m_diffuseTexture = value; return this; }
    Material* setNormalTexture(const std::shared_ptr<Texture>& value) { m_normalTexture = value; return this; }
    Material* setSpecularTexture(const std::shared_ptr<Texture>& value) { m_specularTexture = value; return this; }
    Material* setDisplacementTexture(const std::shared_ptr<Texture>& value) { m_displacementTexture = value; return this; }
    Material* setAlphaTexture(const std::shared_ptr<Texture>& value) { m_alphaTexture = value; return this; }
    Material* setSpecularColor(const glm::vec3& value) { m_specularColor = value; return this; }
    Material* setSpecularPower(float value) { m_specularPower = value; return this; }

    const std::shared_ptr<Texture>& getDiffuseTexture() const { return m_diffuseTexture; }
    const std::shared_ptr<Texture>& getNormalTexture() const { return m_normalTexture; }
    const std::shared_ptr<Texture>& getSpecularTexture() const { return m_specularTexture; }
    const std::shared_ptr<Texture>& getDisplacementTexture() const { return m_displacementTexture; }
    const std::shared_ptr<Texture>& getAlphaTexture() const { return m_alphaTexture; }
    glm::vec3 getSpecularColor() const { return m_specularColor; }
    float getSpecularPower() const { return m_specularPower; }
protected:
private:
    std::shared_ptr<Texture> m_diffuseTexture;
    std::shared_ptr<Texture> m_normalTexture;
    std::shared_ptr<Texture> m_specularTexture;
    std::shared_ptr<Texture> m_displacementTexture;
    std::shared_ptr<Texture> m_alphaTexture;
    glm::vec3 m_specularColor;
    float m_specularPower;

    Material();
};

#endif
