#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <map>
#include <memory>
#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class ResourcesMgr;
class Transform;
class Material;
class Renderer;
class DirectionalLight;
class PointLight;
class SpotLight;

class Shader
{
	friend class ResourcesMgr;
// 	friend class Skybox;
private:
	struct GLSLvariable
	{
		std::string type;
		std::string name;
	};
	enum
	{
		DIFFUSE_MAP = 0,
		NORMAL_MAP,
		SPECULAR_MAP,
		DISPLACEMENT_MAP,
		ALPHA_MAP,
		SKYBOX_MAP,
		SHADOW_MAP
	};
public:
	~Shader();

	void updateCommon(Renderer& renderingEngine);
	void updateTransform(Transform& transform, Renderer& renderingEngine);
	void updateMaterial(std::shared_ptr<Material>& material);

	void bind() const { glUseProgram(m_program); }

	const std::string& getName() const { return m_path; }
protected:
	void setUniformI(const std::string& uniformName, int value) { glUniform1i(m_uniformLocations.at(uniformName), value); }
	void setUniformF(const std::string& uniformName, float value) { glUniform1f(m_uniformLocations.at(uniformName), value); }
	void setUniformV3F(const std::string& uniformName, const glm::vec3& value) { glUniform3f(m_uniformLocations.at(uniformName), value.x, value.y, value.z); }
	void setUniform3F(const std::string& uniformName, float x, float y, float z) { glUniform3f(m_uniformLocations.at(uniformName), x, y, z); }
	void setUniformV2F(const std::string& uniformName, const glm::vec2& value) { glUniform2f(m_uniformLocations.at(uniformName), value.x, value.y); }
	void setUniform2F(const std::string& uniformName, float x, float y) { glUniform2f(m_uniformLocations.at(uniformName), x, y); }
	void setUniformM4F(const std::string& uniformName, const glm::mat4& value) { glUniformMatrix4fv(m_uniformLocations.at(uniformName), 1, GL_FALSE, glm::value_ptr(value)); }
	void setUniform(const std::string& uniformName, const std::shared_ptr<DirectionalLight>& value);
	void setUniform(const std::string& uniformName, const std::shared_ptr<PointLight>& value);
	void setUniform(const std::string& uniformName, const std::shared_ptr<SpotLight>& value);
private:
	Shader(const std::string& path);
	
	GLuint m_program;

	std::map<std::string, int> m_uniformLocations;
	std::vector<GLSLvariable> m_uniforms; //only general GLSLvariables
// 	std::vector<GLSLvariable> m_allUniforms; //all GLSLvariables

	std::string m_path;

	bool addUniform(const GLSLvariable& uniform);
	void addAllUniforms(const std::string& text);

	std::vector<GLSLvariable> parseStruct(const std::string& structName, const std::string& text);

	inline void addVertexShaderFromFile(const std::string& fileName) { addVertexShader(loadShader(fileName)); }
	inline void addGeometryShaderFromFile(const std::string& fileName) { addGeometryShader(loadShader(fileName)); }
	inline void addFragmentShaderFromFile(const std::string& fileName) { addFragmentShader(loadShader(fileName)); }

	inline void addVertexShader(const std::string& text) { addShader(text, GL_VERTEX_SHADER); }
	inline void addGeometryShader(const std::string& text) { addShader(text, GL_GEOMETRY_SHADER); }
	inline void addFragmentShader(const std::string& text) { addShader(text, GL_FRAGMENT_SHADER); }

	void compileShader();
	void addShader(const std::string& text, int type);
	std::string loadShader(const std::string& filePath);
};
#endif // SHADER_H