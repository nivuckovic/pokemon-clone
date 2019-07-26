#include "State.h"

#include "StateStack.h"


State::Context::Context(sf::RenderWindow & window, TextureHolder & textures, Player & player, CharacterCore** characterCore) : m_window(&window), m_textures(&textures), m_player(&player), m_characterCore(characterCore)
{

}

State::State(StateStack & stack, Context context) : m_stack(&stack), m_context(context)
{
}

State::~State()
{
}

void State::requestStackPush(States::ID stateID)
{
	m_stack->pushState(stateID);
}

void State::requestStackPop()
{
	m_stack->popState();
}

void State::requestStateClear()
{
	m_stack->clearStates();
}

State::Context State::getContext() const
{
	return m_context;
}