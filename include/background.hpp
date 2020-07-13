#pragma once

#include <SFML/Graphics.hpp>

class background : public sf::Drawable {
public:
	background();

private:
	virtual void draw(sf::RenderTarget& t, sf::RenderStates s) const;

	sf::Texture m_t;
	sf::RectangleShape m_r;
};