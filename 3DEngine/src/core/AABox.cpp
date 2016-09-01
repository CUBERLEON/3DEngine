#include "AABox.h"

#include <algorithm>

AABox::AABox() : AABox(glm::vec3(0,0,0), glm::vec3(0,0,0))
{}

AABox::AABox(const glm::vec3& pmin, const glm::vec3& pmax) : m_pmin(pmin), m_pmax(pmax)
{}

AABox::~AABox()
{}

void AABox::setBounds(const glm::vec3& pmin, const glm::vec3& pmax) //sets new bounds
{
	m_pmin = pmin;
	m_pmax = pmax;
}

AABox AABox::getTransformed(const glm::mat4& modelMatrix)
{
	glm::vec3 pmin(FLT_MAX, FLT_MAX, FLT_MAX), pmax(-FLT_MAX, -FLT_MAX, -FLT_MAX); //initializing corners of AABox

	glm::vec3 size(m_pmax.x - m_pmin.x, m_pmax.y - m_pmin.y, m_pmax.z - m_pmin.z); //box's size
	glm::vec3 center((m_pmax.x + m_pmin.x) / 2., (m_pmax.y + m_pmin.y) / 2., (m_pmax.z + m_pmin.z) / 2.); //box's center
	int dx[] = {-1, -1, -1, -1, 1, 1, 1, 1}; //all offsets for all corner points of the box (8)
	int dy[] = {-1, -1, 1, 1, -1, -1, 1, 1};
	int dz[] = {-1, 1, -1, 1, -1, 1, -1, 1};

	for (int i = 0; i < 8; ++i)
	{
		glm::vec4 p = (modelMatrix * glm::vec4(center.x + dx[i] * size.x/2., center.y + dy[i] * size.y/2., center.z + dz[i] * size.z/2., 1)); //applying transformation to corner point of the box
				
		//update AABox corners
		pmax.x = std::max(pmax.x, p.x); pmax.y = std::max(pmax.y, p.y); pmax.z = std::max(pmax.z, p.z);
		pmin.x = std::min(pmin.x, p.x); pmin.y = std::min(pmin.y, p.y); pmin.z = std::min(pmin.z, p.z);
	}

	return AABox(pmin, pmax); //returning transformed AABox
}

glm::vec3 AABox::getPositiveVertex(const glm::vec3& normal) const //returns vertex from the box that is further along the normal's direction
{
	glm::vec3 result = m_pmin;
	if (normal.x >= 0)
		result.x = m_pmax.x;
	if (normal.y >= 0)
		result.y = m_pmax.y;
	if (normal.z >= 0)
		result.z = m_pmax.z;
	return result;
}

glm::vec3 AABox::getNegativeVertex(const glm::vec3& normal) const //returns opposite vertex
{
	glm::vec3 result = m_pmax;
	if (normal.x >= 0)
		result.x = m_pmin.x;
	if (normal.y >= 0)
		result.y = m_pmin.y;
	if (normal.z >= 0)
		result.z = m_pmin.z;
	return result;
}
