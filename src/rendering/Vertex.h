#pragma once

#include <glm/glm.hpp>

class Vertex
{
public:
    static const size_t SIZE = 11;

    Vertex(const glm::vec3& position);
    Vertex(const glm::vec3& position, const glm::vec2& texCoord);
    Vertex(const glm::vec3& position, const glm::vec2& texCoord, const glm::vec3& normal);

    ~Vertex();

    inline glm::vec3& getPosition() { return m_position; }
    inline void setPosition(const glm::vec3& m_pos) { m_position = m_pos; }

    inline glm::vec2& getTexCoord() { return m_texCoord; }
    inline void setTexCoord(const glm::vec2& texCoord) { m_texCoord = texCoord; }

    inline glm::vec3& getNormal() { return m_normal; }
    inline void setNormal(const glm::vec3& normal) { m_normal = normal; }

    inline glm::vec3& getTangent() { return m_tangent; }
    inline void setTangent(const glm::vec3& tangent) { m_tangent = tangent; }
protected:
private:
    glm::vec3 m_position;
    glm::vec2 m_texCoord;
    glm::vec3 m_normal;
    glm::vec3 m_tangent;
};

