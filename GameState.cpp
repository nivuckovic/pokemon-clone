#include "GameState.h"

#include "Player.h"

GameState::GameState(StateStack & stack, Context context) : State(stack, context), m_player(*context.m_player), m_world(*context.m_window, context.m_characterCore)
{
}

void GameState::draw()
{
	m_world.draw();
}

bool GameState::update(sf::Time dt)
{
	CommandQueue& m_commandQueue = m_world.getCommandQueue();
	m_player.handleRealtimeEvent(m_commandQueue);
	
	m_world.update(dt);

	return true;
}

bool GameState::handleEvent(const sf::Event & event)
{
	CommandQueue& m_commandQueue = m_world.getCommandQueue();
	m_player.handleEvent(event, m_commandQueue);

	if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
		requestStackPop();

	if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::F5) {
		(*getContext().m_characterCore)->setMap(m_world.getPlayerPosition(), m_world.getLocationID());
		(*getContext().m_characterCore)->save("save", "");
	}

	return true;
}
