#include "Transform.h"

#include "../components/Camera.h"
#include "../core/Debug.h"

Transform::Transform() :
Transform(glm::vec3(0, 0, 0), glm::quat(1, 0, 0, 0), glm::vec3(1, 1, 1))
{
}

Transform::Transform(const glm::vec3& position, const glm::quat& rotation, const glm::vec3& scale)
{
    m_parent = nullptr;

    m_position = position;
    m_rotation = glm::normalize(rotation);
    m_scale = scale;

    updateModelTransform();
    updateRealValues();

    m_oldPosition = m_position;
    m_oldRotation = m_rotation;
    m_oldScale = m_scale;
}

Transform::~Transform()
{}

void Transform::update()
{
    bool changed = hasChanged();

    if (changed) 
    {
        updateModelTransform();

        m_oldPosition = m_position;
        m_oldRotation = m_rotation;
        m_oldScale = m_scale;
    }

    if (changed || (getParent() != nullptr && getParent()->isChildrenUpdateNeeded()))
        updateRealValues();
}

glm::mat4 Transform::getModelViewProjectionTransform(const std::shared_ptr<Camera>& camera) const
{
    return camera->getViewProjectionTransform() * m_realModelTransform;
}

Transform* Transform::rotate(const glm::quat& rotation)
{
    m_rotation = rotation * m_rotation;

    return this;
}

Transform* Transform::translate(const glm::vec3& translation)
{
    m_position += translation;

    return this;
}

Transform* Transform::scale(const glm::vec3& scale)
{
    m_scale *= scale;

    return this;
}

bool Transform::isChildrenUpdateNeeded() const
{
    return m_needChildrenUpdate;
}

glm::vec3 Transform::getLeft() const
{
    return glm::rotate(m_rotation, glm::vec3(-1, 0, 0));
}

glm::vec3 Transform::getRealLeft() const
{
    return glm::rotate(m_realRotation, glm::vec3(-1, 0, 0));
}

glm::vec3 Transform::getRight() const
{
    return glm::rotate(m_rotation, glm::vec3(1, 0, 0));
}

glm::vec3 Transform::getRealRight() const
{
    return glm::rotate(m_realRotation, glm::vec3(1, 0, 0));
}

glm::vec3 Transform::getForward() const
{
    return glm::rotate(m_rotation, glm::vec3(0, 0, -1));
}

glm::vec3 Transform::getRealForward() const
{
    return glm::rotate(m_realRotation, glm::vec3(0, 0, -1));
}

glm::vec3 Transform::getBack() const
{
    return glm::rotate(m_rotation, glm::vec3(0, 0, 1));
}

glm::vec3 Transform::getRealBack() const
{
    return glm::rotate(m_realRotation, glm::vec3(0, 0, 1));
}

glm::vec3 Transform::getUp() const
{
    return glm::rotate(m_rotation, glm::vec3(0, 1, 0));
}

glm::vec3 Transform::getRealUp() const
{
    return glm::rotate(m_realRotation, glm::vec3(0, 1, 0));
}

glm::vec3 Transform::getDown() const
{
    return glm::rotate(m_rotation, glm::vec3(0, -1, 0));
}

glm::vec3 Transform::getRealDown() const
{
    return glm::rotate(m_realRotation, glm::vec3(0, -1, 0));
}

bool Transform::hasChanged()
{
    bool changed = m_oldPosition != m_position ||
                   m_oldRotation != m_rotation ||
                   m_oldScale    != m_scale;
    m_needChildrenUpdate = changed;
    return changed;
}

void Transform::updateRealValues()
{
    if (m_parent != nullptr) 
    {
        m_realRotation = m_parent->getRealRotation() * m_rotation;
        m_realScale = m_scale * m_parent->getRealScale();
        m_realPosition = glm::rotate(m_parent->getRealRotation(), m_position * (m_parent->getRealScale())) + m_parent->getRealPosition();
        m_realModelTransform = m_parent->getRealModelTransform() * m_modelTransform;
    }
    else
    {
        m_realPosition = m_position;
        m_realRotation = m_rotation;
        m_realScale = m_scale;
        m_realModelTransform = m_modelTransform;
    }
}

void Transform::updateModelTransform()
{
    glm::mat4& translation = glm::translate(m_position);
    glm::mat4& rotation = glm::toMat4(m_rotation);
    glm::mat4& scale = glm::scale(m_scale);

    m_modelTransform = translation * (rotation * scale);
}