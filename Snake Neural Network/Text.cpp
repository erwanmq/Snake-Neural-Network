#include "Text.h"


Text::Text(std::string font_file, sf::String text, sf::Vector2f position)
{
	m_font.loadFromFile(font_file);
	m_text.setFont(m_font);
	m_text.setString(text);
	m_text.setPosition(position);
	m_text.setFillColor(sf::Color::Black);
	m_text.setScale(sf::Vector2f(0.5f, 0.5f));
}

Text::~Text() = default;

void Text::setString(sf::String text)
{
	m_text.setString(text);
}

void Text::drawText(sf::RenderWindow& win)
{
	win.draw(m_text);
}
