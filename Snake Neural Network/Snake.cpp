#include "Snake.h"

Snake::Snake()
{
	// initialise the head
	m_head.setFillColor(sf::Color::Red);
	m_head.setSize(m_size);
	//m_head.setOrigin(sf::Vector2f(m_head.getSize().x / 2, m_head.getSize().y / 2));
	m_head.setPosition(sf::Vector2f(1.f * WIDTH / NB_CASE_WIDTH * 15, 1.f * HEIGHT / NB_CASE_HEIGHT * 15));

	// default number of body => 4 squares
	m_body.resize(4);
	// for each square of the body
	for (int i{ 0 }; i < static_cast<int>(m_body.size()); i++)
	{
		// set the size and color
		m_body[i].setSize(m_size);
		m_body[i].setFillColor(sf::Color::Yellow);
		//m_body[i].setOrigin(sf::Vector2f(m_body[i].getSize().x / 2, m_body[i].getSize().y / 2));
		// if it is the first, the position is behind the head
		if (i == 0)
			m_body[i].setPosition(sf::Vector2f(m_head.getPosition().x - m_head.getSize().x, m_head.getPosition().y));
		else // the nexts are just behind the previous body
			m_body[i].setPosition(sf::Vector2f(m_body[i - 1].getPosition().x - m_body[i - 1].getSize().x, m_body[i - 1].getPosition().y));
	}
}

Snake::~Snake() = default;

bool Snake::isThereHisBody()
{
	return true;
}

void Snake::movements(Direction dir)
{
	// change the value of m_direction thanks to the enumerator
	switch (dir)
	{
	case Direction::TOP:
		if (m_direction.y != 1)
		{
			m_direction = sf::Vector2f(0, -1);
		}
		break;
	case Direction::BOTTOM:
		if (m_direction.y != -1)
		{
			m_direction = sf::Vector2f(0, 1);
		}
		break;
	case Direction::LEFT:
		if (m_direction.x != 1)
		{
			m_direction = sf::Vector2f(-1, 0);
		}
		break;
	case Direction::RIGHT:
		if (m_direction.x != -1)
		{
			m_direction = sf::Vector2f(1, 0);
		}
		break;
	}
}

void Snake::update()
{
	// update the lasts body before the firsts 
	for (int i{ static_cast<int>(m_body.size()) - 1 }; i > 0; i--)
	{
		// set the position on the next body
		m_body[i].setPosition(m_body[i - 1].getPosition());
	}
	// set the first square of the body on the head	
	m_body[0].setPosition(m_head.getPosition());
	// set the body one square away to the direction choose
	m_head.setPosition(sf::Vector2f(m_head.getPosition().x + m_direction.x * m_size.x, m_head.getPosition().y + m_direction.y * m_size.y));
}

void Snake::draw(sf::RenderWindow& win, bool isFirst)
{
	// if the snake isn't the first of the game
	if (!isFirst)
	{
		// draw the snake transparent
		m_head.setFillColor(sf::Color(255, 0, 0, 20));
		win.draw(m_head);
		for (sf::RectangleShape& body : m_body)
		{
			body.setFillColor(sf::Color(255, 255, 255, 20));
			win.draw(body);
		}
	}
	else // if the snake is first, draw it completly
	{
		m_head.setFillColor(sf::Color(255, 0, 0, 255));
		win.draw(m_head);
		for (sf::RectangleShape& body : m_body)
		{
			body.setFillColor(sf::Color(255, 255, 255, 255));
			win.draw(body);
		}
			
	}
		
	
}

bool Snake::isHeadInBody()
{
	for (const sf::RectangleShape body : m_body)
	{
		if (m_head.getPosition() == body.getPosition())
			return true;
	}

	return false;
}

void Snake::addBody()
{
	sf::RectangleShape body_temp(m_size);
	body_temp.setFillColor(sf::Color::White);
	//body_temp.setOrigin(sf::Vector2f(body_temp.getSize().x / 2, body_temp.getSize().y / 2));

	body_temp.setPosition(sf::Vector2f(m_body.back().getPosition().x + m_direction.x * m_size.x, m_body.back().getPosition().y + m_direction.y * m_size.y));
	m_body.push_back(body_temp);
}

const sf::Vector2f Snake::getSize() { return m_size; }
const sf::Vector2f Snake::getHeadPosition() { return m_head.getPosition(); }

const sf::RectangleShape Snake::getHead()
{
	return m_head;
}
