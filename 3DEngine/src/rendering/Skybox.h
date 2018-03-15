#pragma once

#include <memory>
#include <string>
#include <GL/glew.h>
#include "../core/NamedObject.h"

class Texture;

class Skybox : public NamedObject
{
public:
    Skybox(const std::string& name);
    ~Skybox();

    void render();
    const std::shared_ptr<Texture>& getCubeMap() const;
protected:
private:
    std::shared_ptr<Texture> m_cubeMap;

    GLuint m_vbo;
};

