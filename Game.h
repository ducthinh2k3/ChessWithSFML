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

    // variables of game
    bool isMove;
    bool isRender;
    int x_start, y_start;
    int x_end, y_end;
    int isTranform;

    // Square direction
    sf::CircleShape squareDirection;
    vector<sf::CircleShape> squareDirections;

    // Private Functions
    void initWindow();
    void initBoard();
    void initVariable();
    void initSquareDirection();

public:
    // constructor, destructor
    Game();
    ~Game(); // note

    // object
    Point getPosition(sf::Vector2f mousePosView);

    // Accessors
    bool running();

    // square Direction
    void createSquareDirections(vector<Point> listValidMoves);

    // Functions
    void pollEvents();
    void updateMousePosition();
    void updateMove();
    void updateTransform();
    void update();
    void renderSquareDirecitons(sf::RenderTarget& target);
    void render();
};