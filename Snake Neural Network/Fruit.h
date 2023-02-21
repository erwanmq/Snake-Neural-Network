#pragma once
#include "Value.h"

class Fruit
{
private:
	sf::RectangleShape m_fruit;
	sf::Vector2f m_position;
	sf::Vector2f m_size;

public:
	Fruit();

	void setSize(sf::Vector2f size);
	void setRandomPosition();
	void drawFruit(sf::RenderWindow& win);
	const sf::Vector2f getPosition();
	const sf::Vector2f getSize();
	const sf::RectangleShape getFruit();
};

