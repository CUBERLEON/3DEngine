#include "Shader.h"

#include <regex>
#include <memory>
#include <fstream>
#include "Texture.h"
#include "Material.h"
#include "Renderer.h"
#include "Skybox.h"
#include "../core/Debug.h"
#include "../core/Transform.h"
#include "../core/Node.h"
#include "../core/ResourcesMgr.h"
#include "../components/Camera.h"
#include "../components/Light.h"
#include "../components/lights/DirectionalLight.h"
#include "../components/lights/PointLight.h"
#include "../components/lights/SpotLight.h"

Shader::Shader(const std::string& path) : m_path(path)
{
    m_program = glCreateProgram();

    std::string vertexShaderSource = loadShader(path + ".vert");
    std::string geometryShaderSource = loadShader(path + ".geom");
    std::string fragmentShaderSource = loadShader(path + ".frag");

    if (!vertexShaderSource.empty())
        addVertexShader(vertexShaderSource);
    if (!geometryShaderSource.empty())
        addGeometryShader(geometryShaderSource);
    if (!fragmentShaderSource.empty())
        addFragmentShader(fragmentShaderSource);

    compileShader();

    addAllUniforms(vertexShaderSource);
    addAllUniforms(geometryShaderSource);
    addAllUniforms(fragmentShaderSource);

    Debug::info("Loaded shader '%s'", m_path.c_str());
}

Shader::~Shader()
{
    glDeleteProgram(m_program);
    Debug::info("Released shader '%s'", m_path.c_str());
}

void Shader::updateCommon(Renderer& renderer)
{
    for (GLSLvariable& variable : m_uniforms)
    {
        switch (variable.name[0])
        {
            //light uniforms
            case 'l':
            {
                if (variable.name == "l_ambientColor")
                    setUniformV3F(variable.name, renderer.getAmbientColor());
                else if (variable.name == "l_directional")
                    setUniform(variable.name, std::dynamic_pointer_cast<DirectionalLight>(renderer.getActiveLight()));
                else if (variable.name == "l_point")
                    setUniform(variable.name, std::dynamic_pointer_cast<PointLight>(renderer.getActiveLight()));
                else if (variable.name == "l_spot")
                    setUniform(variable.name, std::dynamic_pointer_cast<SpotLight>(renderer.getActiveLight()));
                break;
            }
            //other uniforms
            case 'o':
            {
                if (variable.name == "o_vpLightTransform")
                    setUniformM4F(variable.name, renderer.getActiveLight()->getShadowTransform());
                else if (variable.name == "o_shadowMap")
                {
                    setUniformI(variable.name, SHADOW_MAP);
                    renderer.getShadowTexture()->bind(SHADOW_MAP);
                }
                else if (variable.name == "o_skybox")
                {
                    setUniformI(variable.name, SKYBOX_MAP);
                    renderer.getSkybox()->getCubeMap()->bind(SKYBOX_MAP);
                }
                else if (variable.name == "o_skyboxTransform")
                    setUniformM4F(variable.name, renderer.getActiveCamera()->getProjectionTransform() * glm::mat4(glm::mat3(renderer.getActiveCamera()->getViewTransform())));
                else if (variable.name == "o_eyePosition")
                    setUniformV3F(variable.name, renderer.getActiveCamera()->getNode()->getTransform()->getRealPosition());
                break;
            }
        }
    }
}

void Shader::updateTransform(Transform& transform, Renderer& renderingEngine)
{
    for (GLSLvariable& variable : m_uniforms)
    {
        switch (variable.name[0])
        {
            //material uniforms
            case 't':
            {
                if (variable.name == "t_mvpTransform")
                    setUniformM4F("t_mvpTransform", transform.getModelViewProjectionTransform(renderingEngine.getActiveCamera()));
                else if (variable.name == "t_mTransform")
                    setUniformM4F("t_mTransform", transform.getRealModelTransform());
                else
                    break;
            }
        }
    }
}

void Shader::updateMaterial(std::shared_ptr<Material>& material)
{
    for (GLSLvariable& variable : m_uniforms)
    {
        switch (variable.name[0])
        {
            //material uniforms
            case 'm':
            {
                if (variable.name == "m_diffuseMap")
                {
                    setUniformI(variable.name, DIFFUSE_MAP);
                    material->getDiffuseTexture()->bind(DIFFUSE_MAP);
                }
                else if (variable.name == "m_normalMap")
                {
                    setUniformI(variable.name, NORMAL_MAP);
                    material->getNormalTexture()->bind(NORMAL_MAP);
                }
                else if (variable.name == "m_specularMap")
                {
                    setUniformI(variable.name, SPECULAR_MAP);
                    material->getSpecularTexture()->bind(SPECULAR_MAP);
                }
                else if (variable.name == "m_displacementMap")
                {
                    setUniformI(variable.name, DISPLACEMENT_MAP);
                    material->getDisplacementTexture()->bind(DISPLACEMENT_MAP);
                }
                else if (variable.name == "m_alphaMap")
                {
                    setUniformI(variable.name, ALPHA_MAP);
                    material->getAlphaTexture()->bind(ALPHA_MAP);
                }
                else if (variable.name == "m_specularColor")
                    setUniformV3F(variable.name, material->getSpecularColor());
                else if (variable.name == "m_specularPower")
                    setUniformF(variable.name, material->getSpecularPower());
                break;
            }
        }
    }
}

void Shader::setUniform(const std::string& uniformName, const std::shared_ptr<DirectionalLight>& value)
{
    setUniformV3F(uniformName + ".color", value->getColor());
    setUniformF(uniformName + ".intensity", value->getIntensity());
    setUniformV3F(uniformName + ".direction", value->getDirection());
}

void Shader::setUniform(const std::string& uniformName, const std::shared_ptr<PointLight>& value)
{
    setUniformV3F(uniformName + ".color", value->getColor());
    setUniformF(uniformName + ".intensity", value->getIntensity());
    setUniformF(uniformName + ".attenuation.constant", value->getAttenuation().getConstant());
    setUniformF(uniformName + ".attenuation.linear", value->getAttenuation().getLinear());
    setUniformF(uniformName + ".attenuation.exponent", value->getAttenuation().getExponent());
    setUniformV3F(uniformName + ".position", value->getNode()->getTransform()->getPosition());
    setUniformF(uniformName + ".range", value->getRange());
}

void Shader::setUniform(const std::string& uniformName, const std::shared_ptr<SpotLight>& value)
{
    setUniform(uniformName + ".pointLight", std::dynamic_pointer_cast<PointLight>(value));
    setUniformV3F(uniformName + ".direction", value->getDirection());
    setUniformF(uniformName + ".cutoff", value->getCutoff());
}

bool Shader::addUniform(const GLSLvariable& uniform)
{
    int uniformLocation = glGetUniformLocation(m_program, uniform.name.c_str());

    if (uniformLocation == -1)
    {
        Debug::warning("Unnecessary uniform '" + uniform.type + " " + uniform.name + "' in shader '" + m_path + "'");
        return false;
    }
    m_uniformLocations.insert({ uniform.name, uniformLocation });
    return true;
}

void Shader::addAllUniforms(const std::string& text)
{
    std::smatch m;
    std::string subText = text;
    while (std::regex_search(subText, m, std::regex("uniform\\s+([^\\s]+)\\s+([^\\s]+)\\s*;")))
    {
        GLSLvariable uniform({ m[1], m[2] });

        bool used = false;

        std::vector<GLSLvariable> subUniforms = parseStruct(uniform.type, text);
        if (subUniforms.size() == 0)
        {
            used |= addUniform(uniform);
//          m_allUniforms.add(uniform);
        }
        else for (GLSLvariable subUniform : subUniforms)
        {
            used |= addUniform(GLSLvariable({ subUniform.type, uniform.name + "." + subUniform.name }));
//          m_allUniforms.add(new GLSLvariable(subUniform.type, uniform.name + "." + subUniform.name));
        }
        if (used)
            m_uniforms.push_back(uniform);

        subText = m.suffix();
    }
}

std::vector<Shader::GLSLvariable> Shader::parseStruct(const std::string& structName, const std::string& text)
{
    std::vector<GLSLvariable> res;
    std::smatch m;
    if (!std::regex_search(text, m, std::regex("struct\\s+" + structName + "\\s+\\{([^\\}]*)\\};")))
        return res;
    else {
        std::string structFields = m[1];
        while (std::regex_search(structFields, m, std::regex("([^\\s]+)\\s+([^\\s]+)\\s*;")))
        {
            GLSLvariable variable({ m[1], m[2] });

            std::vector<GLSLvariable> subVariables = parseStruct(variable.type, text);
            if (subVariables.size() == 0)
                res.push_back(variable);
            else
            for (GLSLvariable subVariable : subVariables)
                res.push_back(GLSLvariable({ subVariable.type, variable.name + "." + subVariable.name }));

            structFields = m.suffix();
        }

        return res;
    }
}

void Shader::compileShader()
{
    GLint param;
    GLchar info[1024];
    glLinkProgram(m_program);
    glGetProgramiv(m_program, GL_LINK_STATUS, &param);
    if (param == GL_FALSE)
    {
        glGetProgramInfoLog(m_program, 1024, GLsizei(), info);
        Debug::fatalError("Failed to link shader program '" + m_path + "'.\n" + info);
    }

//  glValidateProgram(m_program);
//  glGetProgramiv(m_program, GL_VALIDATE_STATUS, &param);
//  if (param == GL_FALSE)
//  {
//      glGetProgramInfoLog(m_program, 1024, GLsizei(), info);
//      Debug::fatalError("Failed to validate shader program '" + m_path + "'.\n" + info);
//  }
}

void Shader::addShader(const std::string& text, int type)
{
    int shader = glCreateShader(type);

    std::string shaderType;
    if (type == GL_VERTEX_SHADER) shaderType = "vertex";
    else if (type == GL_FRAGMENT_SHADER) shaderType = "fragment";
    else if (type == GL_GEOMETRY_SHADER) shaderType = "geometry";
    else shaderType = std::to_string(type);

    if (shader == 0)
        Debug::fatalError("Could not find valid memory location when adding " + shaderType + " shader '" + m_path + "'");

    GLchar const* files[] = { text.c_str() };
    GLint param;
    glShaderSource(shader, 1, files, NULL);
    glCompileShader(shader);
    glGetShaderiv(shader, GL_COMPILE_STATUS, &param);
    if (param == GL_FALSE)
    {
        GLchar info[1024];
        glGetShaderInfoLog(shader, 1024, GLsizei(), info);
        Debug::fatalError("Compilation of " + shaderType + " shader '" + m_path + "' failed.\n" + info);
    }

    glAttachShader(m_program, shader);
}

std::string Shader::loadShader(const std::string& name)
{
    std::string shaderSource = "";
    try
    {
        std::ifstream file(ResourcesMgr::PATH_PREFIX + "shaders/" + name);
        std::string line;
        while (std::getline(file, line))
        {
            if (line.size() > 7 && line.substr(0, 7) == "include")
            {
                std::smatch m;
                if (std::regex_match(line, m, std::regex("\\s*include\\s+\"([^\"]+)\"\\s*")))
                    shaderSource += loadShader(m[1]);
            }
            else
            {
                shaderSource += line;
            }
            shaderSource += "\n";
        }
    }
    catch (...)//const std::exception& e)
    {
//      Debug::fatalError("Failed to load shader file '" + name + "'. " + e.what());
        return "";
    }

    return shaderSource;
}
