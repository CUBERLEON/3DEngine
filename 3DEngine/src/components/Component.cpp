#include "Component.h"

#include "../core/Node.h"
#include "../core/Transform.h"
#include "../core/Core.h"
#include "../core/Input.h"
#include "../rendering/Window.h"
#include "../core/Debug.h"

Component::Component(const std::string& name) : NamedObject(name), m_core(nullptr)
{}

Component::~Component()
{
    Debug::info("Released component '" + getName() + "'");
}

std::shared_ptr<Node> Component::getNode()
{
    auto ptr = m_node.lock();
//  if (ptr == nullptr)
//      Debug::warning("nullptr returned in Component.getNode(). Node wasn't assigned");
    return ptr;
}
