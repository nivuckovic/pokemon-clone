#pragma once

#include <memory>
#include "SFML\Graphics.hpp"
#include "ResourceHolder.h"
#include "ResourceIdentifier.h"
#include "StateIdentifier.h"
#include "CharacterCore.h"

class Player;
class StateStack;

class State
{
public:
	typedef std::unique_ptr<State> Ptr;

	struct Context {
		Context(sf::RenderWindow& window, TextureHolder& textures, Player& player, CharacterCore** characterCore);

		sf::RenderWindow* m_window;
		TextureHolder* m_textures;
		Player* m_player;
		CharacterCore** m_characterCore;
	};

public:
	State(StateStack& stack, Context context);
	virtual ~State();

	virtual void draw() = 0;
	virtual bool update(sf::Time dt) = 0;
	virtual bool handleEvent(const sf::Event& event) = 0;

protected:
	void requestStackPush(States::ID stateID);
	void requestStackPop();
	void requestStateClear();

	Context getContext() const;

private:
	StateStack * m_stack;
	Context m_context;
};

