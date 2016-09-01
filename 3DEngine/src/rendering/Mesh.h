#pragma once

#include <string>
#include <vector>
#include <memory>
#include <GL/glew.h>

class Vertex;
class Material;
class Shader;
class ResourcesMgr;
class AABox;

class Mesh
{
	friend class ResourcesMgr;
public:
	~Mesh();

	void draw(std::shared_ptr<Shader>& shader);

	AABox* getBounds();
protected:
private:
// 	GLuint m_vao;

	GLuint m_vbo;
	size_t m_verticesCnt;

	std::vector<GLuint> m_ibos;
	std::vector<int> m_iboSizes;
	std::vector< std::shared_ptr<Material> > m_materials;
	size_t m_totalIndicesCnt;

	AABox* m_bounds;

	Mesh(std::vector<Vertex>& vertices, std::vector< std::vector<int> >& indices, std::vector< std::shared_ptr<Material> >& materials, bool needCalcNormals = false, bool hasTexCoords = false);

	void add(std::vector<int>& indices, std::shared_ptr<Material>& material);

	bool m_needCalcNormals;
	void calcNormals(std::vector<Vertex> &vertices, std::vector< std::vector<int> >& indices);
	void calcTangents(std::vector<Vertex> &vertices, std::vector< std::vector<int> >& indices);
};

