#pragma once

#include "SFML\Graphics.hpp"
#include "Animation.h"
#include <assert.h>

class AnimatedSprite : public sf::Drawable, public sf::Transformable
{
public:

	AnimatedSprite(sf::Time timePerFrame = sf::seconds(0.1f), bool isPaused = false, bool isLooped = true );

	void update(sf::Time dt);
	
	void setAnimation(const Animation& animation);
	void setFrame(int n);

	void play();
	void pause();
	void stop();

	sf::FloatRect getLocalBounds() const;

	bool isLooped() const;
	bool isPlaying() const;

private:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
	const Animation* m_animation;
	const sf::Texture* m_texture;
	sf::VertexArray m_vertices;
	sf::Time m_timePerFrame;
	sf::Time m_currentTime;
	int m_currentFrame;
	bool m_isPaused;
	bool m_isLooped;

};

