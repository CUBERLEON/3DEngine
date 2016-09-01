#include "PerspectiveCamera.h"

#include <glm/glm.hpp>
#include "../../core/Core.h"
#include "../../core/Node.h"
#include "../../core/Input.h"
#include "../../core/Transform.h"
#include "../../rendering/Window.h"

PerspectiveCamera::PerspectiveCamera(const std::string& name, float fov, float aspectRatio, float zNear, float zFar) : Camera(name)
{
	m_projectionTransform = glm::perspective(fov, aspectRatio, zNear, zFar);
}

PerspectiveCamera::~PerspectiveCamera()
{}

void PerspectiveCamera::updateRotation(float time)
{
	Input* input = getCore()->getWindow()->getInput();

	glm::vec2 center = getCore()->getWindow()->getCenter();
	glm::vec2 delta = input->getMousePosition() - center;

	bool movedX = delta.x != 0;
	bool movedY = delta.y != 0;

	glm::quat qx, qy, qz;
	float rotateValue = m_sensitivity / 10000;

	if (movedY)
		qx = glm::angleAxis(-rotateValue * delta.y, getNode()->getTransform()->getRight());
	if (movedX)
		qy = glm::angleAxis(-rotateValue * delta.x, glm::vec3(0, 1, 0));

	if (input->getKey(Input::KEY_Q) ^ input->getKey(Input::KEY_E))
	{
		if (input->getKey(Input::KEY_Q))
			qz = glm::angleAxis(15 * rotateValue, getNode()->getTransform()->getBack());
		if (input->getKey(Input::KEY_E))
			qz = glm::angleAxis(-15 * rotateValue, getNode()->getTransform()->getBack());
	}

	getNode()->getTransform()->rotate(qz * (qy * qx));

	if (movedX || movedY)
		input->setMousePosition(center);
}

void PerspectiveCamera::updatePosition(float time)
{
	Input* input = getCore()->getWindow()->getInput();

	float moveValue = m_speed * time;

	if (input->getKey(Input::KEY_W))
		move(getNode()->getTransform()->getForward(), moveValue);
	if (input->getKey(Input::KEY_S))
		move(getNode()->getTransform()->getBack(), moveValue);
	if (input->getKey(Input::KEY_A))
		move(getNode()->getTransform()->getLeft(), moveValue);
	if (input->getKey(Input::KEY_D))
		move(getNode()->getTransform()->getRight(), moveValue);
	if (input->getKey(Input::KEY_SPACE))
		move(glm::vec3(0, 1, 0), moveValue);
	if (input->getKey(Input::KEY_LEFT_SHIFT))
		move(glm::vec3(0, -1, 0), moveValue);
}

void PerspectiveCamera::setProjection(float fov, float aspectRatio, float zNear, float zFar)
{
	m_projectionTransform = glm::perspective(fov, aspectRatio, zNear, zFar);
}
