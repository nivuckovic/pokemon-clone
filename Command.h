#pragma once

#include <functional>
#include <assert.h>
#include <SFML\Graphics.hpp>

class SceneNode;

struct Command {
	Command() {};
	unsigned int category;
	std::function<void(SceneNode&, sf::Time)> action;
};

template <typename GameObject, typename Function>
std::function<void(SceneNode&, sf::Time)> derivedAction(Function fn) {
	return [=](SceneNode& node, sf::Time dt) {
		assert(dynamic_cast<GameObject*>(&node) != nullptr);

		fn(static_cast<GameObject&>(node), dt);
	};
}