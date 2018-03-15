#pragma once

#include <memory>
#include <string>
#include <vector>
#include <map>
#include "Debug.h"

#include "rendering/Texture.h"
#include "components/Component.h"

class Core;
// class Texture;
class Shader;
class EntityCollection;
class Mesh;
class Node;
class Material;
//class Component;
class Skybox;

class ResourcesMgr
{
    friend class Core;
public:
    ResourcesMgr();
    ~ResourcesMgr();

    static const std::string PATH_PREFIX;

    static std::shared_ptr<Shader> shader(const std::string& name);

    static std::vector<std::shared_ptr<Mesh>> meshArray(const std::string& path);
    static std::shared_ptr<Mesh> getMesh(const std::string& key);

    static std::shared_ptr<Material> getMaterial(const std::string& key);
    static std::shared_ptr<Material> material(const std::string& key);
//  static std::shared_ptr<Texture> texture(const std::string& path);
    template <class... Types>
    static std::shared_ptr<Texture> texture(const std::string& path, Types&&... args);
    static std::shared_ptr<Skybox> skybox(const std::string& name);

    static std::shared_ptr<Node> getNode(const std::string& key);
    static std::shared_ptr<Node> node(const std::string& key);

    template <class ComponentType>
    static std::shared_ptr<ComponentType> getComponent(const std::string& key);
    template <class ComponentType, class... Types>
    static std::shared_ptr<ComponentType> createComponent(const std::string& key, Types&&... args);

    static void eraseUnused();
protected:
private:
    static Core* m_core;
    static void setCore(Core* core) { m_core = core; }

    //shaders
    static std::map< std::string, std::shared_ptr<Shader> > m_shaders;

    //meshes and materials
    static std::map< std::string, std::vector<std::weak_ptr<Mesh>> > m_meshArrays;
    static std::map< std::string, std::shared_ptr<Mesh> > m_meshes;

    //materials and textures
    static std::map< std::string, std::shared_ptr<Material> > m_materials;
    static std::map< std::string, std::shared_ptr<Texture> > m_textures;

    //skyboxs
    static std::map< std::string, std::shared_ptr<Skybox> > m_skyboxs;

    //nodes and components
    static std::map< std::string, std::shared_ptr<Node> > m_nodes;
    static std::map< std::string, std::shared_ptr<Component> > m_components;

    template <class Type>
    static bool eraseUnusedAux(std::map< std::string, std::shared_ptr<Type> >& m);
};

template <class... Types>
std::shared_ptr<Texture> ResourcesMgr::texture(const std::string& path, Types&&... args)
{
//  std::string defaultPath = "models/grid.png";

//  if (path.empty())
//      return texture(defaultPath);

    std::string key = path;
    if (m_textures.count(key))
    {
        std::shared_ptr<Texture> ptr = m_textures[key];
        if (ptr)
            return ptr;
    }

    //  std::shared_ptr<Texture> texture = std::make_shared<Texture>();
    std::shared_ptr<Texture> ptr(new Texture(std::forward<Types>(args)...));
    if (ptr->loadFromFile(path))
    {
        m_textures[key] = ptr;
        return ptr;
    }
//  else if (path != defaultPath)
//      return texture(defaultPath);
    else
    {
//      Debug::fatalError("No default texture.");
        Debug::fatalError("Texture '" + path + "' wasn't loaded.");
        return nullptr;
    }
}

template <class ComponentType>
std::shared_ptr<ComponentType> ResourcesMgr::getComponent(const std::string& key)
{
    if (m_components.count(key))
        return std::dynamic_pointer_cast<ComponentType>(m_components.at(key));
    return nullptr;
}

template <class ComponentType, class... Types>
std::shared_ptr<ComponentType> ResourcesMgr::createComponent(const std::string& key, Types&&... args)
{
    if (m_components.count(key))
    {
        std::shared_ptr<Component> tmp = m_components.at(key);
        if (tmp)
        {
            Debug::warning("Returned old component '" + key + "' instead creating new one");
            std::shared_ptr<ComponentType> ptr = std::dynamic_pointer_cast<ComponentType>(tmp);
            return ptr;
        }
    }
    std::shared_ptr<ComponentType> ptr = std::make_shared<ComponentType>(key, std::forward<Types>(args)...);

    std::shared_ptr<Component> tmp = std::dynamic_pointer_cast<Component>(ptr);
    tmp->setCore(m_core);

    m_components[key] = ptr;
    return ptr;
}

template <class Type>
bool ResourcesMgr::eraseUnusedAux(std::map< std::string, std::shared_ptr<Type> >& m)
{
    bool hasRemovedAnything = false;
    for (auto it = m.cbegin(); it != m.end();)
    {
        if (it->second.use_count() == 1)
        {
            it = m.erase(it);
            hasRemovedAnything = true;
        }
        else
            ++it;
    }
    return hasRemovedAnything;
}
