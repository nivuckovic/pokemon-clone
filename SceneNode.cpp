#include "SceneNode.h"

#include "LocationID.h"
#include "Utility.h"

SceneNode::SceneNode(Category::Type category) : m_defaultCategory(category)
{
}

void SceneNode::draw(sf::RenderTarget & renderTarget, sf::RenderStates states) const
{
	states.transform *= getTransform();

	drawCurrent(renderTarget, states);
	drawChildren(renderTarget, states);

	//drawBoundingBox(renderTarget, states);
}

void SceneNode::drawCurrent(sf::RenderTarget & target, sf::RenderStates states) const
{
}

void SceneNode::drawChildren(sf::RenderTarget & renderTarget, sf::RenderStates states) const
{
	for (auto& child : m_children) {
		child->draw(renderTarget, states);
	}
}

void SceneNode::attachChild(Ptr child)
{
	child->m_parent = this;
	m_children.push_back(std::move(child));
}

SceneNode::Ptr SceneNode::detachChild(const SceneNode & node)
{
	auto found = std::find_if(m_children.begin(), m_children.end(), [&node](Ptr & p) { return p.get() == &node; });
	assert(found != m_children.end());

	Ptr result = std::move(*found);
	result->m_parent = nullptr;
	m_children.erase(found);
	return result;
}

void SceneNode::detachChildren()
{
	m_children.clear();
}

void SceneNode::update(sf::Time dt)
{
	updateCurrent(dt);
	updateChildren(dt);
}

void SceneNode::onCommand(Command command, sf::Time dt)
{
	if (command.category == getCategory()) {
		command.action(*this, dt);
	}

	for (auto& child : m_children) {
		child->onCommand(command, dt);
	}
}

sf::Vector2f SceneNode::getWorldPosition() const
{
	return getWorldTransform() * sf::Vector2f();
}

sf::Transform SceneNode::getWorldTransform() const
{
	sf::Transform transform = sf::Transform::Identity;

	for (const SceneNode* node = this; node != nullptr; node = node->m_parent) {
		transform = node->getTransform() * transform;
	}

	return transform;
}

unsigned int SceneNode::getCategory() const
{
	return m_defaultCategory;
}

void SceneNode::checkSceneCollision(SceneNode & sceneGraph, std::set<SceneNode::Pair>& collisionPairs)
{
	if(sceneGraph.getCategory() & (Category::Player | Category::NPC))
		checkNodeCollision(sceneGraph, collisionPairs);

	//poziva cjelo vrijeme m_sceneGraph->checkSceneCollision()
	for (auto& child : sceneGraph.m_children) {
		checkSceneCollision(*child, collisionPairs);
	}
}

void SceneNode::checkNodeCollision(SceneNode & node, std::set<SceneNode::Pair>& collisionPairs)
{
	if (this != &node && collision(*this, node)) {
		collisionPairs.insert(std::minmax(this, &node));
	}
	
	//poziva na djeci od m_sceneGrapha i usporeduje sa nodom
	for (auto& child : m_children) {
		child->checkNodeCollision(node, collisionPairs);
	}
}

sf::FloatRect SceneNode::getBoundingBox() const
{
	return sf::FloatRect(0, 0, 0, 0);
}

void SceneNode::updateCurrent(sf::Time dt)
{
}

void SceneNode::updateChildren(sf::Time dt)
{
	for (auto& child : m_children) {
		child->update(dt);
	}
}

void SceneNode::drawBoundingBox(sf::RenderTarget & target, sf::RenderStates states) const
{
	sf::FloatRect rect = getWorldTransform().getInverse().transformRect(getBoundingBox());
	
	sf::RectangleShape shape;
	shape.setPosition(sf::Vector2f(rect.left, rect.top));
	shape.setSize(sf::Vector2f(rect.width, rect.height));
	shape.setFillColor(sf::Color::Transparent);
	shape.setOutlineColor(sf::Color::Green);
	shape.setOutlineThickness(0.5f);

	target.draw(shape, states);
}

bool collision(const SceneNode & lhs, const SceneNode & rhs)
{
	return lhs.getBoundingBox().intersects(rhs.getBoundingBox());
}