#include "Application.h"

#include "ResourceHolder.h"
#include "SpriteNode.h"
#include <memory>
#include "LocationID.h"
#include "StateIdentifier.h"

#include "GameState.h"


Application::Application() : m_window(sf::VideoMode(960, 960), "PokeClone"), m_stateStack(State::Context(m_window, m_textures, m_playerController, &m_characterCore))
{
	m_characterCore = new CharacterCore();
	m_characterCore->load("save");

	m_stateStack.registerState<GameState>(States::ID::Game);
	m_stateStack.pushState(States::ID::Game);
}

Application::~Application()
{
	delete m_characterCore;
}

void Application::run()
{
	sf::Clock clock;
	sf::Time timeSinceLastUpdate = sf::Time::Zero;

	sf::Time timePerFrame = sf::seconds(1.f / 60.f);

	bool updated = false;

	while (m_window.isOpen()) {
		sf::Time dt = clock.restart();
		timeSinceLastUpdate += dt;
		
		handleInput();

		updated = false;

		while (timeSinceLastUpdate >= timePerFrame) {
			updated = true;

			timeSinceLastUpdate -= timePerFrame;
			update(timePerFrame);

			if (m_stateStack.isEmpty()) {
				m_window.close();
			}
		}

		if (1) {
			render();
		}
	}
}

void Application::handleInput()
{

	sf::Event e;
	while (m_window.pollEvent(e)) {

		m_stateStack.handleEvent(e);

		if (e.type == sf::Event::Closed) {
			m_window.close();
		}
	}
}

void Application::update(sf::Time dt)
{
	m_stateStack.update(dt);
}

void Application::render()
{
	m_window.clear();
	m_stateStack.draw();
	m_window.display();
}
