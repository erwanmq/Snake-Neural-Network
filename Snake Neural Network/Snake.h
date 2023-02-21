#pragma once
#include <vector>
#include "NeuralNetwork.h"
#include <SFML/Graphics.hpp>
#include "Value.h"


class Snake
{
private:
	// vector of each square of the body
	std::vector<sf::RectangleShape> m_body;
	sf::RectangleShape m_head; // square of the head

	// size of a body of the snake
	sf::Vector2f m_size{ sf::Vector2f(1.f * WIDTH / NB_CASE_WIDTH, 1.f * HEIGHT / NB_CASE_HEIGHT) };

	sf::Vector2f m_position;

	sf::Vector2f m_direction{ sf::Vector2f(1, 0) };
	Direction m_currentDir;
	int m_score{ 0 };

private:
	void setPosition();

public:
	Snake();
	~Snake();

	void movements(Direction dir);
	void update();
	void draw(sf::RenderWindow& win, bool isFirst);
	bool isHeadInBody();
	void addBody();
	const sf::Vector2f getSize();
	const sf::Vector2f getHeadPosition();
	const sf::RectangleShape getHead();
	bool isThereHisBody();
	Direction getDirection();

	std::vector<sf::Vector2f> getAllBodyPosition();
};

