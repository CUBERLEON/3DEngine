#pragma once

#include <string>
#include <GL/glew.h>

class ResourcesMgr;

class Texture
{
friend class ResourcesMgr;
friend class Renderer;
public:
    ~Texture();

    void bind(int samplerLocation);
    void setImage2D(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const void *pixels);
    void setWrap(GLint wrap);
    void setMinFilter(GLint minFilter);
    void setMagFilter(GLint magFilter);
protected:
private:
    GLuint m_id;

    GLenum m_target;

    GLint m_minFilter;
    GLint m_magFilter;
    GLint m_wrap;

    std::string m_path;

    Texture(GLenum target = GL_TEXTURE_2D, GLint minFilter = GL_LINEAR_MIPMAP_LINEAR, GLint magFileter = GL_LINEAR, GLint wrap = GL_REPEAT);

    bool loadFromFile(const std::string& path);
    static unsigned char* loadRawImage(const std::string& path, int& width, int& height);

    GLuint getID() const { return m_id; }
    void setParameteri(GLenum pname, GLint param);
};

