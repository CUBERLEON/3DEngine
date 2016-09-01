/*Axis aligned bounding box class*/

#pragma once

#include <glm/glm.hpp>

class AABox
{
public:
	AABox();
	AABox(const glm::vec3& pmin, const glm::vec3& pmax);
	~AABox();

	void setBounds(const glm::vec3& pmin, const glm::vec3& pmax); //sets new bounds
	AABox getTransformed(const glm::mat4& modelMatrix); //returns transformed AABox (applied model matrix)

	const glm::vec3& getMinPoint() const { return m_pmin; }
	const glm::vec3& getMaxPoint() const { return m_pmax; }

	glm::vec3 getPositiveVertex(const glm::vec3& normal) const; //returns vertex from the box that is further along the normal's direction
	glm::vec3 getNegativeVertex(const glm::vec3& normal) const; //returns opposite vertex
protected:
private:
	glm::vec3 m_pmin; //min point
	glm::vec3 m_pmax; //max point
};