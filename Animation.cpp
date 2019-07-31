#include "Animation.h"

Animation::Animation(const sf::Texture & texture, int row, int size, int width, int height) : m_texture(&texture)
{
	for (int i = 1; i < size; ++i) {
		addFrame(sf::IntRect(i * width, height * row, width, height));
	}
}

void Animation::addFrame(sf::IntRect rect)
{
	m_frames.push_back(rect);
}

const sf::Texture * Animation::getSpriteSheet() const
{
	return m_texture;
}

int Animation::getSize() const
{
	return m_frames.size();
}

sf::IntRect Animation::getFrame(int n) const
{
	return m_frames[n];
}
