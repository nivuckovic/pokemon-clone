#pragma once

#include "SFML\Graphics.hpp"

#include "ResourceIdentifier.h"
#include "ResourceHolder.h"

#include "Human.h"
#include "CommandQueue.h"
#include "Player.h"
#include "World.h"
#include "StateStack.h"
#include "CharacterCore.h"

class Application {
public:
	Application();
	~Application();
	
	void run();

private:
	void handleInput();
	void update(sf::Time dt);
	void render();

private:
	sf::RenderWindow m_window;
	TextureHolder m_textures;
	Player m_playerController;
	CharacterCore* m_characterCore;

	StateStack m_stateStack;
};

