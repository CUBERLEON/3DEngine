#include "ResourcesMgr.h"

#include <regex>
#include <glm/glm.hpp>
#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>
#include "../components/Light.h"
#include "../components/Camera.h"
#include "../components/Entity.h"
#include "../rendering/Texture.h"
#include "../rendering/Shader.h"
#include "../rendering/Mesh.h"
#include "../rendering/Material.h"
#include "../rendering/Vertex.h"
#include "../rendering/Skybox.h"
#include "Node.h"
#include "Debug.h"

ResourcesMgr::ResourcesMgr()
{}

ResourcesMgr::~ResourcesMgr()
{}

const std::string ResourcesMgr::PATH_PREFIX = "./res/";

std::shared_ptr<Shader> ResourcesMgr::shader(const std::string& name)
{
	if (m_shaders.count(name))
	{
		std::shared_ptr<Shader> ptr = m_shaders[name];
		if (ptr)
			return ptr;
	}

// 	std::shared_ptr<Shader> ptr = std::make_shared<Shader>(name);
	std::shared_ptr<Shader> ptr(new Shader(name));
	m_shaders[name] = ptr;

	return ptr;
}

std::vector<std::shared_ptr<Mesh>> ResourcesMgr::meshArray(const std::string& path)
{
	if (m_meshArrays.count(path))
	{
		std::vector< std::weak_ptr<Mesh> > weakArray = m_meshArrays[path];

		std::vector< std::shared_ptr<Mesh> > array;
		bool valid = true;
		for (size_t i = 0; i < weakArray.size() && valid; ++i)
		{
			array.push_back(weakArray[i].lock());
			if (!array[i])
				valid = false;
		}
		if (valid)
			return array;
	}

	std::vector< std::shared_ptr<Mesh> > array;

	std::string fullPath = PATH_PREFIX + path;
// 	try 
	{
		std::smatch m;
		if (!std::regex_match(path, m, std::regex("(.+[\\|/])[^\\/]+")))
			throw std::runtime_error("Invalid path.");
		std::string folderPath = m[1];

		std::vector<tinyobj::shape_t> loadedShapes;
		std::vector<tinyobj::material_t> loadedMaterials;

		std::string message;
		if (!tinyobj::LoadObj(loadedShapes, loadedMaterials, message, fullPath.c_str(), (PATH_PREFIX + folderPath).c_str()))
			throw std::runtime_error(message);

		if (message.length() > 0)
			Debug::warning(message);

		std::vector< std::shared_ptr<Material> > materials(loadedMaterials.size());
		for (size_t i = 0; i < loadedMaterials.size(); ++i)
		{
			materials[i] = material(path + ">" + loadedMaterials[i].name);
			materials[i]->setSpecularColor(glm::vec3(loadedMaterials[i].specular[0], loadedMaterials[i].specular[1], loadedMaterials[i].specular[2]));
			materials[i]->setSpecularPower(loadedMaterials[i].shininess);
			if (!loadedMaterials[i].diffuse_texname.empty())
				materials[i]->setDiffuseTexture(ResourcesMgr::texture(folderPath + loadedMaterials[i].diffuse_texname, GL_TEXTURE_2D, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR, GL_REPEAT));
			if (!loadedMaterials[i].bump_texname.empty())
				materials[i]->setNormalTexture(ResourcesMgr::texture(folderPath + loadedMaterials[i].bump_texname, GL_TEXTURE_2D, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR, GL_REPEAT));
			if (!loadedMaterials[i].specular_texname.empty())
				materials[i]->setSpecularTexture(ResourcesMgr::texture(folderPath + loadedMaterials[i].specular_texname, GL_TEXTURE_2D, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR, GL_REPEAT));
			if (!loadedMaterials[i].displacement_texname.empty())
				materials[i]->setDisplacementTexture(ResourcesMgr::texture(folderPath + loadedMaterials[i].displacement_texname, GL_TEXTURE_2D, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR, GL_REPEAT));
			if (!loadedMaterials[i].alpha_texname.empty())
				materials[i]->setAlphaTexture(ResourcesMgr::texture(folderPath + loadedMaterials[i].alpha_texname, GL_TEXTURE_2D, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR, GL_REPEAT));
		}

		for (size_t i = 0; i < loadedShapes.size(); ++i)
		{
			size_t numberOfVertices = loadedShapes[i].mesh.positions.size() / 3;
			size_t numberOfIndices = loadedShapes[i].mesh.indices.size();
			bool shapeHasTexcoords = numberOfVertices == loadedShapes[i].mesh.texcoords.size() / 2;
			bool shapeHasNormals = numberOfVertices == loadedShapes[i].mesh.normals.size() / 3;

			if (!numberOfVertices || !numberOfIndices)
				continue;

			std::vector<Vertex> shapeVertices;

			for (size_t j = 0; j < numberOfVertices; ++j)
			{
				Vertex tmp(glm::vec3(loadedShapes[i].mesh.positions[3 * j], loadedShapes[i].mesh.positions[3 * j + 1], loadedShapes[i].mesh.positions[3 * j + 2]));
				if (shapeHasTexcoords)
					tmp.setTexCoord(glm::vec2(loadedShapes[i].mesh.texcoords[2 * j], 1 - loadedShapes[i].mesh.texcoords[2 * j + 1]));
				if (shapeHasNormals)
					tmp.setNormal(glm::vec3(loadedShapes[i].mesh.normals[3 * j], loadedShapes[i].mesh.normals[3 * j + 1], loadedShapes[i].mesh.normals[3 * j + 2]));

				shapeVertices.push_back(tmp);
			}

			std::vector< std::vector<int> > shapeIndices;
			std::vector< std::shared_ptr<Material> > shapeMaterials;
			int k = 0;

			int materialID = loadedShapes[i].mesh.material_ids[0], nextMaterialID = -1;
			for (size_t j = 0; 3 * j < numberOfIndices; ++j)
			{
				if (shapeIndices.size() == k)
				{
					shapeIndices.push_back({});
					shapeMaterials.push_back({});
				}
				shapeIndices[k].push_back(loadedShapes[i].mesh.indices[3 * j]);
				shapeIndices[k].push_back(loadedShapes[i].mesh.indices[3 * j + 1]);
				shapeIndices[k].push_back(loadedShapes[i].mesh.indices[3 * j + 2]);

				materialID = loadedShapes[i].mesh.material_ids[j];
				nextMaterialID = (j + 1 < loadedShapes[i].mesh.material_ids.size()) ? loadedShapes[i].mesh.material_ids[j + 1] : -1;

				if (materialID != nextMaterialID || 3 * j == numberOfIndices - 3)
				{
					std::shared_ptr<Material> shapeMaterial;
					if (materialID == -1 || materialID < 0 || materialID >= (int)materials.size())
						shapeMaterial = material("default");
					else
						shapeMaterial = materials[materialID];

					shapeMaterials[k] = shapeMaterial;
					++k;
				}
			}

			std::shared_ptr<Mesh> shapeMesh(new Mesh(shapeVertices, shapeIndices, shapeMaterials, !shapeHasNormals, shapeHasTexcoords));

			array.push_back(shapeMesh);
			m_meshes[path + ">" + loadedShapes[i].name] = shapeMesh;
		}
	}
// 	catch (const std::exception& r)
// 	{
// 		Debug::error("Failed to load all objects from file " + fullPath + ". " + r.what());
// 	}

	std::vector< std::weak_ptr<Mesh> > weakArray(array.size());
	for (size_t i = 0; i < array.size(); ++i)
		weakArray.push_back(array[i]);
	m_meshArrays[path] = weakArray;

	return array;
}

std::shared_ptr<Mesh> ResourcesMgr::getMesh(const std::string& key)
{
	if (m_meshes.count(key))
		return m_meshes.at(key);
	return nullptr;
}

std::shared_ptr<Material> ResourcesMgr::getMaterial(const std::string& key)
{
	if (m_materials.count(key))
		return m_materials.at(key);
	return nullptr;
}

std::shared_ptr<Material> ResourcesMgr::material(const std::string& key)
{
	if (m_materials.count(key))
	{
		std::shared_ptr<Material> ptr = m_materials.at(key);
		if (ptr)
		{
// 			Debug::warning("Returned old material '" + key + "' instead creating new one");
			return ptr;
		}
	}
	std::shared_ptr<Material> ptr(new Material());
	m_materials[key] = ptr;
	return ptr;
}

std::shared_ptr<Skybox> ResourcesMgr::skybox(const std::string& name)
{
	std::string key = name;
	if (m_skyboxs.count(key))
	{
		std::shared_ptr<Skybox> ptr = m_skyboxs[key];
		if (ptr)
			return ptr;
	}
	std::shared_ptr<Skybox> ptr(new Skybox(name));
	return ptr;
}

std::shared_ptr<Node> ResourcesMgr::getNode(const std::string& key)
{
	if (m_nodes.count(key))
		return m_nodes.at(key);
	return nullptr;
}

std::shared_ptr<Node> ResourcesMgr::node(const std::string& key)
{
	if (m_nodes.count(key))
	{
		std::shared_ptr<Node> ptr = m_nodes.at(key);
		if (ptr)
		{
// 			Debug::warning("Returned old node '" + key + "' instead creating new one");
			return ptr;
		}
	}
	std::shared_ptr<Node> ptr(new Node(key));
	ptr->setCore(m_core);

	m_nodes[key] = ptr;
	return ptr;
}

void ResourcesMgr::eraseUnused()
{
	while (eraseUnusedAux(m_nodes));
	eraseUnusedAux(m_components);
	eraseUnusedAux(m_meshes);
	eraseUnusedAux(m_materials);
	eraseUnusedAux(m_textures);
	eraseUnusedAux(m_shaders);
	eraseUnusedAux(m_skyboxs);
}

Core* ResourcesMgr::m_core = nullptr;

std::map< std::string, std::shared_ptr<Shader> > ResourcesMgr::m_shaders;

std::map< std::string, std::vector<std::weak_ptr<Mesh>> > ResourcesMgr::m_meshArrays;
std::map< std::string, std::shared_ptr<Mesh> > ResourcesMgr::m_meshes;

std::map< std::string, std::shared_ptr<Material> > ResourcesMgr::m_materials;
std::map< std::string, std::shared_ptr<Texture> > ResourcesMgr::m_textures;

std::map< std::string, std::shared_ptr<Skybox> > ResourcesMgr::m_skyboxs;

std::map< std::string, std::shared_ptr<Node> > ResourcesMgr::m_nodes;
std::map< std::string, std::shared_ptr<Component> > ResourcesMgr::m_components;