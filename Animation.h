#pragma once

#include <SFML\Graphics.hpp>
#include <vector>

class Animation
{
public:
	Animation(const sf::Texture& texture, int row, int size, int width, int height);

	void addFrame(sf::IntRect rect);
	const sf::Texture* getSpriteSheet() const;
	int getSize() const;
	sf::IntRect getFrame(int n) const;
	//dodati ako zatreba

private:
	std::vector <sf::IntRect> m_frames;
	const sf::Texture* m_texture;
};

