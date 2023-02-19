#pragma once
#include <vector>
#include "NeuralNetwork.h"
#include <SFML/Graphics.hpp>

// size of the window
constexpr int WIDTH{ 700 };
constexpr int HEIGHT{ 700 };

constexpr int NB_CASE_WIDTH{30 / 700 * WIDTH};
constexpr int NB_CASE_HEIGHT{30 / 700 * HEIGHT};

class Snake
{
private:
	// vector of each square of the body
	std::vector<sf::RectangleShape> m_body;
	sf::RectangleShape m_head; // square of the head

	// size of a body of the snake
	sf::Vector2f m_size{ sf::Vector2f(0.02f * WIDTH, 0.02f * HEIGHT) };

	sf::Vector2f m_position;

	sf::Vector2f m_direction{ sf::Vector2f(1, 0) };
	int m_score{ 0 };

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
};

