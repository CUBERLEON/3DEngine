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

	inline Material* setDiffuseTexture(std::shared_ptr<Texture>& value) { m_diffuseTexture = value; return this; }
	inline Material* setNormalTexture(std::shared_ptr<Texture>& value) { m_normalTexture = value; return this; }
	inline Material* setSpecularTexture(std::shared_ptr<Texture>& value) { m_specularTexture = value; return this; }
	inline Material* setDisplacementTexture(std::shared_ptr<Texture>& value) { m_displacementTexture = value; return this; }
	inline Material* setAlphaTexture(std::shared_ptr<Texture>& value) { m_alphaTexture = value; return this; }
	inline Material* setSpecularColor(const glm::vec3& value) { m_specularColor = value; return this; }
	inline Material* setSpecularPower(float value) { m_specularPower = value; return this; }

	inline const std::shared_ptr<Texture>& getDiffuseTexture() const { return m_diffuseTexture; }
	inline const std::shared_ptr<Texture>& getNormalTexture() const { return m_normalTexture; }
	inline const std::shared_ptr<Texture>& getSpecularTexture() const { return m_specularTexture; }
	inline const std::shared_ptr<Texture>& getDisplacementTexture() const { return m_displacementTexture; }
	inline const std::shared_ptr<Texture>& getAlphaTexture() const { return m_alphaTexture; }
	inline glm::vec3 getSpecularColor() const { return m_specularColor; }
	inline float getSpecularPower() const { return m_specularPower; }
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