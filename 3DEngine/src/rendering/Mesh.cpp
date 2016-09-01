#include "Mesh.h"

#include <algorithm>
#include <glm/glm.hpp>
#include <GL/glew.h>
#include "Vertex.h"
#include "../core/AABox.h"
#include "Material.h"
#include "Shader.h"
#include "Renderer.h"
#include "../core/Debug.h"

Mesh::Mesh(std::vector<Vertex>& vertices, std::vector< std::vector<int> >& indices, std::vector< std::shared_ptr<Material> >& materials, bool needCalcNormals, bool hasTexCoords)
: m_needCalcNormals(needCalcNormals), m_verticesCnt(vertices.size()), m_totalIndicesCnt(0), m_bounds(new AABox())
{
	if (needCalcNormals)
		calcNormals(vertices, indices);
	if (hasTexCoords)
		calcTangents(vertices, indices);

	glm::vec3 pmin(FLT_MAX, FLT_MAX, FLT_MAX), pmax(-FLT_MAX, -FLT_MAX, -FLT_MAX);
	for (size_t i = 0; i < vertices.size(); ++i)
	{
		glm::vec3& p = vertices[i].getPosition();
		pmax.x = std::max(pmax.x, p.x); pmax.y = std::max(pmax.y, p.y); pmax.z = std::max(pmax.z, p.z);
		pmin.x = std::min(pmin.x, p.x); pmin.y = std::min(pmin.y, p.y); pmin.z = std::min(pmin.z, p.z);
	}
	m_bounds->setBounds(pmin, pmax);

// 	glGenVertexArrays(1, &m_vao);
	glGenBuffers(1, &m_vbo);

// 	glBindVertexArray(m_vao);

	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	if (vertices.size() > 0)
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertices[0]), &(vertices[0]), GL_STATIC_DRAW);

	for (size_t i = 0; i < indices.size(); ++i)
		add(indices[i], materials[i]);

// 	glEnableVertexAttribArray(0);
// 	glEnableVertexAttribArray(1);
// 	glEnableVertexAttribArray(2);
// 
// 	glVertexAttribPointer(0, 3, GL_FLOAT, false, 4 * 8, (void*)(0));
// 	glVertexAttribPointer(1, 2, GL_FLOAT, false, 4 * 8, (void*)(4 * 3));
// 	glVertexAttribPointer(2, 3, GL_FLOAT, false, 4 * 8, (void*)(4 * (3 + 2)));
// 
// 	glBindVertexArray(0);
// 	glDisableVertexAttribArray(0);
// 	glDisableVertexAttribArray(1);
// 	glDisableVertexAttribArray(2);
}

Mesh::~Mesh()
{
	delete m_bounds;

// 	glDeleteVertexArrays(1, &m_vao);

	glDeleteBuffers(1, &m_vbo);
	for (size_t i = 0; i < m_materials.size(); ++i)
		glDeleteBuffers(1, &(m_ibos[i]));
	Debug::info("Released mesh (vertices_cnt=%d, indices_cnt=%d)", m_verticesCnt, m_totalIndicesCnt);
}

void Mesh::draw(std::shared_ptr<Shader>& shader)
{
	if (!m_ibos.size())
		return;

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);

// 	glBindVertexArray(m_vao);

	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, false, 4 * Vertex::SIZE, (void*)(0));
	glVertexAttribPointer(1, 2, GL_FLOAT, false, 4 * Vertex::SIZE, (void*)(4 * 3));
	glVertexAttribPointer(2, 3, GL_FLOAT, false, 4 * Vertex::SIZE, (void*)(4 * (3 + 2)));
	glVertexAttribPointer(3, 3, GL_FLOAT, false, 4 * Vertex::SIZE, (void*)(4 * (3 + 2 + 3)));

	for (size_t i = 0; i < m_ibos.size(); ++i)
	{
		shader->updateMaterial(m_materials[i]);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibos[i]);
		glDrawElements(GL_TRIANGLES, m_iboSizes[i], GL_UNSIGNED_INT, 0);
	}

// 	glBindVertexArray(0);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(3);
}

AABox* Mesh::getBounds()
{
	return m_bounds;
}

void Mesh::add(std::vector<int>& indices, std::shared_ptr<Material>& material)
{
	for (size_t i = 0; i < indices.size(); ++i)
		if (indices[i] >= (int)m_verticesCnt)
		{
			Debug::error("Indices exceed number of vertices in the mesh. Skipping...");
			return;
		}
	if (indices.size() > 0)
	{
		GLuint ibo;
		glGenBuffers(1, &ibo);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(int), &(indices[0]), GL_STATIC_DRAW);

		m_ibos.push_back(ibo);
		m_iboSizes.push_back((int)indices.size());
		m_totalIndicesCnt += indices.size();
		m_materials.push_back(material);
	}
}

void Mesh::calcNormals(std::vector<Vertex> &vertices, std::vector< std::vector<int> >& indices)
{
	for (Vertex& vertex : vertices)
		vertex.setNormal(glm::vec3(0, 0, 0));

	for (size_t k = 0; k < indices.size(); ++k)
	{
		for (size_t i = 0; i < indices[k].size(); i += 3)
		{
			int i0 = indices[k][i];
			int i1 = indices[k][i + 1];
			int i2 = indices[k][i + 2];

			glm::vec3 v1 = vertices[i1].getPosition() - vertices[i0].getPosition();
			glm::vec3 v2 = vertices[i2].getPosition() - vertices[i0].getPosition();

			glm::vec3 normal = glm::normalize(glm::cross(v1, v2));

			vertices[i0].getNormal() += normal;
			vertices[i1].getNormal() += normal;
			vertices[i2].getNormal() += normal;
		}
	}

	for (Vertex& vertex : vertices)
		vertex.setNormal(glm::normalize(vertex.getNormal()));
}

void Mesh::calcTangents(std::vector<Vertex> &vertices, std::vector< std::vector<int> >& indices)
{
	for (Vertex& vertex : vertices)
		vertex.setTangent(glm::vec3(0, 0, 0));

	for (size_t k = 0; k < indices.size(); ++k)
	{
		for (size_t i = 0; i < indices[k].size(); i += 3)
		{
			int i0 = indices[k][i];
			int i1 = indices[k][i + 1];
			int i2 = indices[k][i + 2];

			glm::vec3 e1 = vertices[i1].getPosition() - vertices[i0].getPosition();
			glm::vec3 e2 = vertices[i2].getPosition() - vertices[i0].getPosition();

			glm::vec2 uv1 = vertices[i1].getTexCoord() - vertices[i0].getTexCoord();
			glm::vec2 uv2 = vertices[i2].getTexCoord() - vertices[i0].getTexCoord();

			float r = 1.f / (uv1.x * uv2.y - uv1.y * uv2.x + 1e-7f);

			glm::vec3 tangent = r * (e1 * uv2.y - e2 * uv1.y);

			vertices[i0].getTangent() += tangent;
			vertices[i1].getTangent() += tangent;
			vertices[i2].getTangent() += tangent;
		}
	}

	for (Vertex& vertex : vertices)
		vertex.setTangent(glm::normalize(vertex.getTangent()));
}
