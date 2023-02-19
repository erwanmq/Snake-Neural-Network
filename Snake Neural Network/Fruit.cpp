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
	m_position = sf::Vector2f(std::rand() % (WIDTH - 2 * static_cast<int>(m_size.x) + 1) + m_size.x, std::rand() % (HEIGHT - 2 * static_cast<int>(m_size.y) + 1) + m_size.y);
	m_fruit.setPosition(m_position);
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
