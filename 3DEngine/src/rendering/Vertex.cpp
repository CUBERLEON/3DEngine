#include "Vertex.h"

#include "../core/Debug.h"

Vertex::Vertex(const glm::vec3& position)
: Vertex(position, glm::vec2(-1, -1), glm::vec3(0, 0, 0))
{}

Vertex::Vertex(const glm::vec3& position, const glm::vec2& texCoord)
    : Vertex(position, texCoord, glm::vec3(0, 0, 0))
{}

Vertex::Vertex(const glm::vec3& position, const glm::vec2& texCoord, const glm::vec3& normal)
: m_position(position), m_texCoord(texCoord), m_normal(normal), m_tangent(0, 0, 0)
{}

Vertex::~Vertex()
{}
