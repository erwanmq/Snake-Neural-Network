#include "Fruit.h"

Fruit::Fruit()
{
	m_fruit.setFillColor(sf::Color::Red);
}

void Fruit::setSize(sf::Vector2f size)
{
	m_size = size;
	m_fruit.setSize(m_size);
}
void Fruit::setRandomPosition()
{
	m_position = sf::Vector2f(std::rand() % (NB_CASE_WIDTH + 1), std::rand() % (NB_CASE_HEIGHT + 1));
	m_fruit.setPosition(sf::Vector2f(1.f * WIDTH / NB_CASE_WIDTH * m_position.x, 1.f * HEIGHT / NB_CASE_HEIGHT * m_position.y));
}

void Fruit::drawFruit(sf::RenderWindow& win, bool isFirst)
{
	if (!isFirst)
	{
		// draw the snake transparent
		m_fruit.setFillColor(sf::Color(255, 0, 0, 20));
		win.draw(m_fruit);
	}
	else // if the snake is first, draw it completly
	{
		m_fruit.setFillColor(sf::Color(255, 0, 0, 255));
		win.draw(m_fruit);
	}
}

const sf::Vector2f Fruit::getPosition() { return m_position; }
const sf::Vector2f Fruit::getSize() { return m_size; }

const sf::RectangleShape Fruit::getFruit()
{
	return m_fruit;
}
