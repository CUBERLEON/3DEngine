#ifndef CAMERA_H
#define CAMERA_H

#include <string>
#include <glm/glm.hpp>
#include <algorithm>
#include "Component.h"

class Transform;
class Core;
class Frustum;

class Camera : public Component, public std::enable_shared_from_this<Camera>
{
public:
	void move(const glm::vec3& direction, float value);

	void activate();
	bool isActive();

	const glm::mat4& getViewProjectionTransform();
	const glm::mat4& getViewTransform();
	const glm::mat4& getProjectionTransform();

	inline bool isLocked() const { return m_isLocked; }
	inline float getSpeed() const { return m_speed; }
	inline float getSensitivity() const { return m_sensitivity; }

	inline void setLock(bool lock) { m_isLocked = lock; }
	inline void setSpeed(float speed) { m_speed = std::max(0.5f, speed); }
	inline void setSensitivity(float sensitivity) { m_sensitivity = std::max(0.5f, sensitivity); }

	Frustum* getFrustum();
protected:
	Camera(const std::string& name);
	virtual ~Camera();

	Frustum* m_frustum;

	bool m_isLocked;

	float m_sensitivity;
	float m_speed;

	glm::mat4 m_viewTransform;
	glm::mat4 m_projectionTransform;
	glm::mat4 m_viewProjectionTransform;

	virtual void updateRotation(float time) = 0;
	virtual void updatePosition(float time) = 0;
	void updateViewProjectionTransform();
private:
	void update(float time) override;
};

#endif // CAMERA_H
