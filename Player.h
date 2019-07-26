#pragma once

#include <SFML\Graphics.hpp>
#include <map>

#include "Command.h"

class CommandQueue;

class Player {
public:
	enum Action {
		MoveLeft,
		MoveRight,
		MoveUp,
		MoveDown,
		Count
	};

public:
	Player();

	void handleEvent(const sf::Event & event, CommandQueue & commandQueue);
	void handleRealtimeEvent(CommandQueue & commandQueue);

	void assignKey(Action action, sf::Keyboard::Key key);
	sf::Keyboard::Key getAssignedKey(Action action) const;

private:
	void initializeActions();
	static bool isRealtimeAction(Action action);

private:
	std::map<sf::Keyboard::Key, Player::Action> m_keyBinding;
	std::map<Player::Action, Command> m_actionBinding;

};

