#pragma once

#include <SFML\Graphics.hpp>
#include <vector>
#include <memory>
#include "Command.h"
#include "Category.h"

#include <set>

class SceneNode : public sf::Transformable, public sf::Drawable, private sf::NonCopyable
{
public:
	typedef std::unique_ptr<SceneNode> Ptr;
	typedef std::pair<SceneNode*, SceneNode*> Pair;
public:
	SceneNode(Category::Type category = Category::Type::None);

	void attachChild(Ptr child);
	SceneNode::Ptr detachChild(const SceneNode& node);
	void detachChildren();

	void update(sf::Time dt);

	void onCommand(Command command, sf::Time dt);

	sf::Vector2f getWorldPosition() const;
	sf::Transform getWorldTransform() const;

	virtual unsigned int getCategory() const;

	void checkSceneCollision(SceneNode& sceneGraph, std::set<SceneNode::Pair>& collisionPairs);
	void checkNodeCollision(SceneNode& node, std::set<SceneNode::Pair>& collisionPairs);

	virtual sf::FloatRect getBoundingBox() const;

private:
	virtual void updateCurrent(sf::Time dt);
	void updateChildren(sf::Time dt);

	virtual void draw(sf::RenderTarget& renderTarget, sf::RenderStates states) const;

	virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;
	void drawChildren(sf::RenderTarget& renderTarget, sf::RenderStates states) const;

	void drawBoundingBox(sf::RenderTarget& target, sf::RenderStates states) const;

private:
	std::vector<Ptr> m_children;
	SceneNode* m_parent;

	Category::Type m_defaultCategory;
};

bool collision(const SceneNode& lhs, const SceneNode& rhs);