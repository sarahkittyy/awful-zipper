#include "background.hpp"

background::background() {
	m_t.loadFromFile("assets/bg.jpg");
	m_r.setTexture(&m_t);
	m_r.setSize(sf::Vector2f(800, 600));
}

void background::draw(sf::RenderTarget& t, sf::RenderStates s) const {
	t.draw(m_r, s);
}