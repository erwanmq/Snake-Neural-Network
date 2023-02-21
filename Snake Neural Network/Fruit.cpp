#include "Fruit.h"

Fruit::Fruit()
{
	m_fruit.setFillColor(sf::Color::Blue);
}

void Fruit::setSize(sf::Vector2f size)
{
	m_size = size;
	m_fruit.setSize(m_size);
}
void Fruit::setRandomPosition()
{
	m_position = sf::Vector2f(static_cast<float>( 1 + (std::rand()) % (NB_CASE_WIDTH - 3)), static_cast<float>( 1 + (std::rand()) % (NB_CASE_HEIGHT - 3)));
	m_fruit.setPosition(sf::Vector2f(CONV_CASE_WIDTH * m_position.x, CONV_CASE_HEIGHT * m_position.y));
}

void Fruit::drawFruit(sf::RenderWindow& win)
{
	win.draw(m_fruit);
}

const sf::Vector2f Fruit::getPosition() { return m_position; }
const sf::Vector2f Fruit::getSize() { return m_size; }

const sf::RectangleShape Fruit::getFruit()
{
	return m_fruit;
}
