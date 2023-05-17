#include "Game.h"


int main()
{
    // creation of the window
    constexpr int MAIN_WINDOW_WIDTH{ WIDTH + 200 };
    constexpr int MAIN_WINDOW_HEIGHT{ HEIGHT };
    sf::RenderWindow window(sf::VideoMode(MAIN_WINDOW_WIDTH, MAIN_WINDOW_HEIGHT), "Snake Neural Network");
    window.setFramerateLimit(20);

    srand((unsigned int)time(0)); // seed the random generator 

    Game game(1000); // create a game with 100 players

    sf::RectangleShape black_background;
    black_background.setSize(sf::Vector2f(WIDTH, HEIGHT));
    black_background.setFillColor(sf::Color::Black);
    black_background.setPosition(sf::Vector2f(0, 0));

    // main loop
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        
        game.update(); // update the game

        window.clear(sf::Color::White);
        
        window.draw(black_background);
        game.drawGame(window); // draw the content
            
        window.display();
    }

    return 0;
}
