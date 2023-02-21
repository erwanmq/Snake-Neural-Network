#include "Grid.h"
#include "Text.h"

void Grid::initObjectsGrid()
{
	for (int i{ 0 }; i < static_cast<int>(m_objects.size()); i++)
	{
		for (int j{ 0 }; j < static_cast<int>(m_objects[i].size()); j++)
		{
			if (i == static_cast<int>(m_objects.size()) - 1 || j == static_cast<int>(m_objects[i].size()) - 1 || i == 0 || j == 0)
				m_objects[i][j] = 1;
			else
				m_objects[i][j] = 0;

		}
	}
}

Grid::Grid()
{
	sf::RectangleShape line;
	line.setFillColor(sf::Color::White);
	line.setSize(sf::Vector2f(1.f, HEIGHT));
	for (int i{ 0 }; i < NB_CASE_WIDTH; i++)
	{
		line.setPosition(sf::Vector2f(1.f * WIDTH / NB_CASE_WIDTH * i, 0));
		m_lines.push_back(line);
	}

	line.setSize(sf::Vector2f(WIDTH, 1.f));
	for (int i{ 0 }; i < NB_CASE_HEIGHT; i++)
	{
		line.setPosition(sf::Vector2f(0, 1.f * HEIGHT / NB_CASE_HEIGHT * i));
		m_lines.push_back(line);
	}
	m_objects.resize(NB_CASE_WIDTH, std::vector<int>(NB_CASE_HEIGHT));
	initObjectsGrid();
}

void Grid::addObjectInTheGrid(int value, int column, int line) // 0 nothing, 1 wall, 2 fruit
{
	m_objects[column][line] = value;
}

int Grid::getObjectInTheGrid(int column, int line)
{
	return m_objects[column][line];
}

bool Grid::isThereAWall(int column, int line, Direction dir)
{
	switch (dir)
	{
	case Direction::RIGHT:
		if (m_objects[column + 1][line] == 1)
			return true;
		break;
	case Direction::LEFT:
		if (m_objects[column - 1][line] == 1)
			return true;
		break;
	case Direction::TOP:
		if (m_objects[column][line - 1] == 1)
			return true;
		break;
	case Direction::BOTTOM:
		if (m_objects[column][line + 1] == 1)
			return true;
		break;
	}
	return false;
}

bool Grid::isThereAFruit(int column, int line, Direction dir)
{
	switch (dir)
	{
	case Direction::RIGHT:
		for (int i{ column }; i < m_objects.size(); i++)
		{
			if (m_objects[i][line] == 2)
				return true;
		}
		break;
	case Direction::LEFT:
		for (int i{ 0 }; i < column; i++)
		{
			if (m_objects[i][line] == 2)
				return true;
		}
		break;
	case Direction::TOP:
		for (int i{ 0 }; i < line; i++)
		{
			if (m_objects[column][i] == 2)
				return true;
		}
		break;
	case Direction::BOTTOM:
		for (int i{ line }; i < m_objects.size(); i++)
		{
			if (m_objects[column][i] == 2)
				return true;
		}
		break;
	}
	return false;
}

void Grid::drawGrid(sf::RenderWindow& win)
{
	for (const auto& line : m_lines)
		win.draw(line);

	for (int i{ 0 }; i < m_objects.size(); i++)
	{
		for (int j{ 0 }; j < m_objects[i].size(); j++)
		{
			Text text(std::string("DejaVuSansMono.ttf"), sf::String(std::to_string(m_objects[i][j])), sf::Vector2f(1.f * WIDTH / NB_CASE_WIDTH * i, 1.f * HEIGHT / NB_CASE_HEIGHT * j));
			text.drawText(win);
		}
	}
	initObjectsGrid();
		
}
