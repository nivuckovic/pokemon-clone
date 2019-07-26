#include "Player.h"

#include "Human.h"
#include "CommandQueue.h"
#include "Category.h"
#include <assert.h>


struct PlayerMover {
	PlayerMover(float x, float y, Directions direction) {
		m_direction.x = x;
		m_direction.y = y;

		m_dir = direction;
	};

	void operator() (Human & human, sf::Time dt) const {
		human.setMoveDirection(m_direction, m_dir);
	};

	sf::Vector2f m_direction;
	Directions m_dir;
};

Player::Player()
{
	m_keyBinding[sf::Keyboard::Left] = Action::MoveLeft;
	m_keyBinding[sf::Keyboard::Right] = Action::MoveRight;
	m_keyBinding[sf::Keyboard::Up] = Action::MoveUp;
	m_keyBinding[sf::Keyboard::Down] = Action::MoveDown;

	initializeActions();

	for (auto& action : m_actionBinding) {
		action.second.category = Category::Player;
	}
}

void Player::handleEvent(const sf::Event & event, CommandQueue & commandQueue)
{
	if (event.type == sf::Event::KeyPressed) {
		auto found = m_keyBinding.find(event.key.code);
		if (found != m_keyBinding.end() && isRealtimeAction(found->second)) {
			commandQueue.push(m_actionBinding[found->second]);
		}
	}
}

void Player::handleRealtimeEvent(CommandQueue & commandQueue)
{
	for (auto pair : m_keyBinding) {
		if (sf::Keyboard::isKeyPressed(pair.first) && isRealtimeAction(pair.second)) {
			commandQueue.push(m_actionBinding[pair.second]);
		}
	}
}

void Player::assignKey(Action action, sf::Keyboard::Key key)
{
	auto found = m_keyBinding.find(key);
	if (found != m_keyBinding.end()) {
		found->second = action;
	}
	else {
		m_keyBinding[key] = action;
	}
}

sf::Keyboard::Key Player::getAssignedKey(Action action) const
{
	for (auto pair : m_keyBinding) {
		if (pair.second == action) {
			return pair.first;
		}
	}

	return sf::Keyboard::Unknown;
}

void Player::initializeActions()
{
	m_actionBinding[Action::MoveDown].action = derivedAction<Human>(PlayerMover(0, 16, Directions::Down));
	m_actionBinding[Action::MoveUp].action = derivedAction<Human>(PlayerMover(0, -16, Directions::Top));
	m_actionBinding[Action::MoveRight].action = derivedAction<Human>(PlayerMover(16, 0, Directions::Right));
	m_actionBinding[Action::MoveLeft].action = derivedAction<Human>(PlayerMover(-16, 0, Directions::Left));
}

bool Player::isRealtimeAction(Action action)
{
	switch (action)
	{
	case MoveLeft:
	case MoveRight:
	case MoveDown:
	case MoveUp:
		return true;

	default:
		return false;
	}
}
