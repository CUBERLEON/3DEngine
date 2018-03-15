#include "Camera.h"

#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/transform.hpp>
#include "../core/Transform.h"
#include "../core/Core.h"
#include "../core/Input.h"
#include "../core/Node.h"
#include "../core/Frustum.h"
#include "../rendering/Renderer.h"
#include "../rendering/Window.h"
#include "../core/Debug.h"

Camera::Camera(const std::string& name) : Component(name), m_isLocked(false), m_sensitivity(8), m_speed(6),
m_frustum(new Frustum())
{}

Camera::~Camera()
{
    delete m_frustum;
}

void Camera::updateViewProjectionTransform()
{
    glm::mat4 cameraTranslation = glm::translate(-getNode()->getTransform()->getRealPosition());
    glm::mat4 cameraRotation = glm::transpose(glm::toMat4(getNode()->getTransform()->getRealRotation()));
    m_viewTransform = cameraRotation * cameraTranslation;

    m_viewProjectionTransform = m_projectionTransform * m_viewTransform;
}

const glm::mat4& Camera::getViewProjectionTransform()
{
    return m_viewProjectionTransform;
}

const glm::mat4& Camera::getViewTransform()
{
    return m_viewTransform;
}

const glm::mat4& Camera::getProjectionTransform()
{
    return m_projectionTransform;
}

Frustum* Camera::getFrustum()
{
    return m_frustum;
}

void Camera::move(const glm::vec3& direction, float value)
{
    getNode()->getTransform()->translate(glm::normalize(direction) * value);
}

void Camera::activate()
{
    getCore()->getRenderingEngine()->setCamera(shared_from_this());
}

bool Camera::isActive()
{
    return getCore()->getRenderingEngine()->getActiveCamera() == shared_from_this();
}

void Camera::update(float time)
{
    updateViewProjectionTransform();
    m_frustum->setViewProjectionMatrix(m_viewProjectionTransform);

    Input* input = getCore()->getWindow()->getInput();

    if (getCore()->getWindow()->isFocused() && isActive())
    {
        if (!m_isLocked)
        {
            updatePosition(time);
            updateRotation(time);
        }

        if (input->isKeyPressed(Input::KEY_ESCAPE))
        {
            m_isLocked = !m_isLocked;
            input->setMousePosition(getCore()->getWindow()->getCenter());
            input->setCursor(m_isLocked);
        }
    }
}
