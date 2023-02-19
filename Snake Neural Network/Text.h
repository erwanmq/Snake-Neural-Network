#pragma once
#include <SFML/Graphics.hpp>
#include <string>

class Text
{
private:
	sf::Font m_font;
	sf::Text m_text;

public:
	Text(std::string font_file, sf::String text, sf::Vector2f position);
	~Text();

	void setString(sf::String text);
	void drawText(sf::RenderWindow& win);
};

