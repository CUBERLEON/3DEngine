#pragma once

#include <string>
#include <vector>
#include <set>
#include <memory>
#include <GL/glew.h>
#include <glm/glm.hpp>

class Node;
class Camera;
class Light;
class Shader;
class Window;
class Texture;
class Skybox;

class Renderer
{
//  friend class Shader;
//  friend class Core;
//  friend class Light;
//  friend class Camera;
//  friend class Scene;
public:
    Renderer(Window* window);
    ~Renderer();

    void render(std::shared_ptr<Node>& root);

    void setCamera(const std::shared_ptr<Camera>& mainCamera);
    void setSkybox(const std::shared_ptr<Skybox>& skybox);
    void addLight(const std::shared_ptr<Light>& light);
    bool removeLight(const std::shared_ptr<Light>& light);

    std::string getOpenGLVersion();
    inline const glm::vec3& getAmbientColor() const { return m_ambientColor; }
    const std::shared_ptr<Camera>& getActiveCamera() const;
    const std::shared_ptr<Light>& getActiveLight() const;
    const std::shared_ptr<Skybox>& getSkybox() const;
    Texture* getShadowTexture() { return m_shadowMap; }
protected:
private:
    Window* m_window;

    std::shared_ptr<Camera> m_activeCamera;

    glm::vec3 m_ambientColor;
    std::set< std::shared_ptr<Light> > m_lights;
    std::shared_ptr<Light> m_activeLight;

//  std::shared_ptr<Shader> m_ambientShader;
    std::shared_ptr<Shader> m_debugShader;

    GLuint m_shadowWidth;
    GLuint m_shadowHeight;
    Texture* m_shadowMap;
    GLuint m_shadowMapFBO;
    std::shared_ptr<Shader> m_depthShader;

    std::shared_ptr<Shader> m_skyboxShader;
    std::shared_ptr<Skybox> m_skybox;

    void renderAux(std::shared_ptr<Node>& root, std::shared_ptr<Shader>& shader);
};

