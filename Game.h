#pragma once
#include "Board.h"

class Game
{
private:
    // Private variables
    sf::RenderWindow window;
    sf::VideoMode videoMode;
    sf::Event ev;

    // Mouse Position
    sf::Vector2i mousePosWindow;
    sf::Vector2f mousePosView;

    // object
    Board* board;

    // Private Functions
    void initWindow();
    void initBoard();
public:
    // constructor, destructor
    Game();
    ~Game(); // note

    // Accessors
    bool running();

    // Functions
    void pollEvents();
    void updateMousePosition();
    void update();
    void render();
};