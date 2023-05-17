#pragma once
#include <SFML/Graphics.hpp>


// enumerator to keep track of each direction
enum class Direction
{
	RIGHT,
	LEFT,
	TOP,
	BOTTOM
};

// size of the window
constexpr int WIDTH{ 700 };
constexpr int HEIGHT{ 700 };

constexpr int NB_CASE_WIDTH{ 15 };
constexpr int NB_CASE_HEIGHT{ 15 };

constexpr float CONV_CASE_WIDTH{ 1.f * WIDTH / NB_CASE_WIDTH };
constexpr float CONV_CASE_HEIGHT{ 1.f * HEIGHT / NB_CASE_HEIGHT };

// CONVERSION CASE -> POSITION => CONV_CASE_WIGHT * nb_case
// CONVERSION POSITION -> CASE => position / CONV_CASE_WIDTH (static_cast<int>())