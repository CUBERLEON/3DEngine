#ifndef COMPONENT_H
#define COMPONENT_H

#include <memory>
#include "../core/NamedObject.h"

class Transform;
class Node;
class Renderer;
class Shader;
class Core;
class Window;
class Input;
class ResourcesMgr;

class Component : public NamedObject
{
friend class Node;
friend class ResourcesMgr;
public:
    std::shared_ptr<Node> getNode();
protected:
    Component(const std::string& name);
    virtual ~Component();

    Core* getCore() { return m_core; }
private:
    Core* m_core;
    std::weak_ptr<Node> m_node;

    void setCore(Core* core) { m_core = core; }
    void setNode(const std::shared_ptr<Node>& node) { m_node = node; }

    virtual void update(float time) {}
    virtual void render(const std::shared_ptr<Shader>& shader, Renderer& renderingEngine) {}

    virtual void addedToSceneEvent() {}
    virtual void removedFromSceneEvent() {}
};

#endif // COMPONENT_H
