#ifndef NODE_H
#define NODE_H

#include <vector>
#include <map>
#include <set>
#include <memory>
#include "NamedObject.h"

class Core;
class Component;
class Transform;
class Shader;
class Renderer;
class ResourcesMgr;

class Node : public NamedObject, public std::enable_shared_from_this<Node>
{
    friend class Renderer;
    friend class Scene;
    friend class Component;
    friend class ResourcesMgr;
public:
    Node(const std::string& name);
    ~Node();

    std::shared_ptr<Node> attach(const std::shared_ptr<Node>& node);
    std::shared_ptr<Node> attach(const std::vector< std::shared_ptr<Node> >& nodes);
    bool detach(const std::shared_ptr<Node>& node);
    bool hasChild(const std::shared_ptr<Node>& node);

    std::shared_ptr<Node> addComponent(const std::shared_ptr<Component>& component);
    std::shared_ptr<Node> addComponents(const std::vector< std::shared_ptr<Component> >& components);
    bool removeComponent(const std::shared_ptr<Component>& component);
    bool hasComponent(const std::shared_ptr<Component>& component);

    inline const std::set< std::shared_ptr<Node> >& getChildren() const { return m_children; }
    inline const std::set< std::shared_ptr<Component> >& getComponents() const { return m_components; }

    inline Transform* getTransform() const { return m_transform; };
    inline std::shared_ptr<Node> getParent() const { return m_parent.lock(); }
protected:
private:
    std::set< std::shared_ptr<Node> > m_children;
    std::set< std::shared_ptr<Component> > m_components;

    Transform* m_transform;

    void update(float time);
    void render(const std::shared_ptr<Shader>& shader, Renderer& renderingEngine);

    Core* m_core;
    inline void setCore(Core* core) { m_core = core; }

    std::weak_ptr<Node> m_parent;
    inline void setParent(const std::shared_ptr<Node>& parent) { m_parent = parent; }
};

#endif // NODE_H
