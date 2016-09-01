#include "Node.h"

#include "Debug.h"
#include "Transform.h"
#include "Core.h"
#include "../rendering/Renderer.h"
#include "../components/Light.h"
#include "../components/Component.h"

Node::Node(const std::string& name) : NamedObject(name), m_core(nullptr), m_transform(new Transform())
{}

Node::~Node()
{
	delete m_transform;
	Debug::info("Released node '" + getName() + "'");
}

std::shared_ptr<Node> Node::attach(const std::shared_ptr<Node>& node)
{
	auto oldParent = node->getParent();
	if (oldParent == shared_from_this())
		return shared_from_this();

	if (oldParent)
		oldParent->detach(node);

	node->setParent(shared_from_this());
	node->getTransform()->setParent(m_transform);
	m_children.insert(node);

	return shared_from_this();
}

std::shared_ptr<Node> Node::attach(const std::vector< std::shared_ptr<Node> >& nodes)
{
	for (size_t i = 0; i < nodes.size(); ++i)
		attach(nodes[i]);

	return shared_from_this();
}

bool Node::detach(const std::shared_ptr<Node>& node)
{
	auto it = m_children.find(node);
	if (it != m_children.end())
	{
		m_children.erase(it);
		node->setParent(nullptr);
		node->getTransform()->setParent(nullptr);
		return true;
	}
	return false;
}

bool Node::hasChild(const std::shared_ptr<Node>& node)
{
// 	return m_children.find(node) != m_children.end();
	return node->getParent() == shared_from_this();
}

std::shared_ptr<Node> Node::addComponent(const std::shared_ptr<Component>& component)
{
	auto oldNode = component->getNode();
	if (oldNode == shared_from_this())
		return shared_from_this();

	if (oldNode)
		oldNode->removeComponent(component);

	component->setNode(shared_from_this());
	component->addedToSceneEvent();

	m_components.insert(component);

	return shared_from_this();
}

std::shared_ptr<Node> Node::addComponents(const std::vector< std::shared_ptr<Component> >& components)
{
	for (size_t i = 0; i < components.size(); ++i)
		addComponent(components[i]);

	return shared_from_this();
}

bool Node::removeComponent(const std::shared_ptr<Component>& component)
{
	auto it = m_components.find(component);
	if (it != m_components.end())
	{
		m_components.erase(it);

		component->setNode(nullptr);
		component->removedFromSceneEvent();

		return true;
	}
	return false;
}

bool Node::hasComponent(const std::shared_ptr<Component>& component)
{
// 	return m_components.find(component) != m_components.end();
	return component->getNode() == shared_from_this();
}

void Node::update(float time)
{
	m_transform->update();

	for (auto& component : m_components)
		component->update(time);

	for (auto& child : m_children)
		child->update(time);
}

void Node::render(std::shared_ptr<Shader>& shader, Renderer& renderingEngine)
{
	for (auto& component : m_components)
		component->render(shader, renderingEngine);

	for (auto& child : m_children)
		child->render(shader, renderingEngine);
}