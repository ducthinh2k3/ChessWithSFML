#include "Game.h"
#include "PieceFactory.h"

// Private Funtions
void Game::initWindow()
{
    this->videoMode.height = 800;
    this->videoMode.width = 1200;
    this->window.create(this->videoMode, "My window", sf::Style::Default);
    this->window.setFramerateLimit(144);
}

// Object
void Game::initBoard()
{
    this->board = new Board;
}

// Contructor, Destructor
Game::Game()
{
    this->initWindow();
    this->initBoard();
}

Game::~Game()
{
    delete this->board;
}

// Accessors
bool Game::running()
{
    return this->window.isOpen() && this->board->endGame() == false;
}

// Functions

void Game::pollEvents()
{
    while (this->window.pollEvent(this->ev))
    {
        switch (this->ev.type)
        {
        case sf::Event::Closed:
            this->window.close();
            break;
        case sf::Event::MouseButtonPressed:
            if (ev.key.code == sf::Mouse::Left)
            {
                if (this->board->isTransform == true)
                {
                    this->board->transformPawn(this->mousePosView);
                }
                else
                {
                    this->board->savePointStart(this->mousePosView);
                }
            }
            break;
        case sf::Event::MouseButtonReleased:
            if (ev.key.code == sf::Mouse::Left)
            {
                this->board->Move(this->mousePosView);
            }
            break;
        default:
            break;
        }
    }
}

void Game::updateMousePosition()
{
    this->mousePosWindow = sf::Mouse::getPosition(this->window);
    this->mousePosView = this->window.mapPixelToCoords(this->mousePosWindow);
    // cout << this->mousePosView.x << " " << this->mousePosView.y << endl;
}


void Game::update()
{
    // check zoom window
    if (this->window.getSize().x != 1200 || this->window.getSize().y != 800)
        this->board->isRender = true;

    this->updateMousePosition();

    this->pollEvents();

    this->board->updateMove(this->mousePosView);
}

void Game::render()
{
    //cout << this->board->isRender << endl;
    if (this->board->isRender)
    {
        this->board->isRender = false;

        // Clear
        this->window.clear(sf::Color::Black);

        // Draw
        this->board->renderBoard(this->window);

        // Draw square direction
        this->board->renderSquareDirecitons(this->window);

        // Display
        this->window.display();
    }
}
