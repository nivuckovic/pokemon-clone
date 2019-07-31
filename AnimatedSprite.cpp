#include "AnimatedSprite.h"
#include "Utility.h"

AnimatedSprite::AnimatedSprite(sf::Time timePerFrame, bool isPaused, bool isLooped) : m_timePerFrame(timePerFrame), m_isPaused(isPaused), m_isLooped(isLooped)
{
	m_vertices.setPrimitiveType(sf::Quads);
	m_vertices.resize(4);
}

void AnimatedSprite::update(sf::Time dt)
{
	if (!m_isPaused && m_animation) {
		m_currentTime += dt;

		if (m_currentTime >= m_timePerFrame) 
		{
			m_currentTime = sf::microseconds(m_currentTime.asMicroseconds() % m_timePerFrame.asMicroseconds());

			if (m_currentFrame + 1 < m_animation->getSize()) {
				++m_currentFrame;
			}
			else {
				m_currentFrame = 0;

				if (!m_isLooped) {
					m_isPaused = true;
				}

			}

			setFrame(m_currentFrame);
		}
	}
}

void AnimatedSprite::setAnimation(const Animation & animation)
{
	if (this->m_animation == &animation)
		return;

	m_animation = &animation;
	m_texture = m_animation->getSpriteSheet();
	m_currentFrame = 0;
	setFrame(m_currentFrame);
	//promjeniti 8 u offset koji prima
	setOrigin(getLocalBounds().width / 2, (getLocalBounds().height + 8.f ) / 2);
}

void AnimatedSprite::setFrame(int n)
{
	if (m_animation) {
		sf::IntRect rect = m_animation->getFrame(n);

		m_vertices[0].position = sf::Vector2f(0.f, 0.f);
		m_vertices[1].position = sf::Vector2f(0.f, (float)(rect.height));
		m_vertices[2].position = sf::Vector2f((float)(rect.width), (float)(rect.height));
		m_vertices[3].position = sf::Vector2f((float)(rect.width), 0.f);

		float left = (float)(rect.left);
		float right = left + (float)(rect.width);
		float top = (float)(rect.top);
		float bottom = top + (float)(rect.height);

		m_vertices[0].texCoords = sf::Vector2f(left, top);
		m_vertices[1].texCoords = sf::Vector2f(left, bottom);
		m_vertices[2].texCoords = sf::Vector2f(right, bottom);
		m_vertices[3].texCoords = sf::Vector2f(right, top);
	}
}

void AnimatedSprite::play()
{
	m_isPaused = false;
}

void AnimatedSprite::pause()
{
	m_isPaused = true;
}

void AnimatedSprite::stop()
{
	m_isPaused = true;
	m_currentFrame = 0;
	setFrame(m_currentFrame);
}

sf::FloatRect AnimatedSprite::getLocalBounds() const
{
	sf::IntRect rect = m_animation->getFrame(m_currentFrame);

	float width = (float)(rect.width);
	float height = (float)(rect.height);

	return getTransform().transformRect(sf::FloatRect(0.f, 0.f, width, height));
}

bool AnimatedSprite::isLooped() const
{
	return m_isLooped;
}

bool AnimatedSprite::isPlaying() const
{
	return !m_isPaused;
}

void AnimatedSprite::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	if (m_animation && m_texture) {
		states.transform *= getTransform();
		states.texture = m_texture;
		target.draw(m_vertices, states);
	}
}
