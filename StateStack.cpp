#include "StateStack.h"

State::Ptr StateStack::createState(States::ID stateID)
{
	auto found = m_factories.find(stateID);
	assert(found != m_factories.end());

	return found->second();
}

void StateStack::applyPendingChanges()
{
	for (auto& change : m_pendingList) {
		switch (change.m_action)
		{
		case Action::Push:
			m_stack.push_back(createState(change.m_stateID));
			break;

		case Action::Pop:
			m_stack.pop_back();
			break;

		case Action::Clear:
			m_stack.clear();
			break;

		default:
			break;
		}
	}

	m_pendingList.clear();
}

StateStack::StateStack(State::Context context) : m_context(context)
{
}

void StateStack::update(sf::Time dt)
{
	for (auto itr = m_stack.rbegin(); itr != m_stack.rend(); ++itr) {
		if (!(*itr)->update(dt)) {
			return;
		}
	}

	applyPendingChanges();
}

void StateStack::draw()
{
	for (auto& state : m_stack) {
		state->draw();
	}
}

void StateStack::handleEvent(const sf::Event & event)
{
	for (auto itr = m_stack.rbegin(); itr != m_stack.rend(); ++itr) {
		if (!(*itr)->handleEvent(event)) {
			return;
		}
	}

	applyPendingChanges();
}

void StateStack::pushState(States::ID stateID)
{
	m_pendingList.push_back(PendingChange(Action::Push, stateID));
}

void StateStack::popState()
{
	m_pendingList.push_back(PendingChange(Action::Pop));
}

void StateStack::clearStates()
{
	m_pendingList.push_back(PendingChange(Clear));
}

bool StateStack::isEmpty() const
{
	return m_stack.empty();
}

StateStack::PendingChange::PendingChange(Action action, States::ID stateID) : m_action(action), m_stateID(stateID)
{

}
