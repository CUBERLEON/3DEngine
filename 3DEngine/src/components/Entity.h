#pragma once

#include <memory>
#include <vector>
#include <string>
#include "Component.h"

class Mesh;
class Shader;
class Renderer;

class Entity : public Component, public std::enable_shared_from_this<Entity>
{
public:
	Entity(const std::string& name);
	~Entity();

	std::shared_ptr<Entity> addMesh(const std::shared_ptr<Mesh>& mesh);
	std::shared_ptr<Entity> addMeshes(const std::vector< std::shared_ptr<Mesh> >& mesh);
protected:
private:
	std::vector< std::shared_ptr<Mesh> > m_meshes;

	void render(std::shared_ptr<Shader>& shader, Renderer& renderingEngine) override;
};

