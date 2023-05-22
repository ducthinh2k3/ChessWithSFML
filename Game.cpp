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

Point Game::getPosition(sf::Vector2f mousePosView)
{
    Point location;
    location.x = (int)mousePosView.x / 100;
    location.y = (int)mousePosView.y / 100;
    return location;
}

// Variable
void Game::initVariable()
{
    this->isMove = false;
    this->isRender = true;
    x_start = y_start = x_end = y_end = 0;
    this->isTranform = false;
}

// Square Direction
void Game::initSquareDirection()
{
    this->squareDirection.setRadius(10);
    this->squareDirection.setFillColor(sf::Color(192,192,192));
    this->squareDirection.setOrigin(squareDirection.getRadius(), squareDirection.getRadius());
}

void Game::createSquareDirections(vector<Point> listValidMoves)
{
    this->squareDirections.erase(this->squareDirections.begin(), this->squareDirections.end());
    for (int i = 0; i < listValidMoves.size(); i++)
    {
        this->squareDirection.setPosition(listValidMoves[i].y * 100 + 50, listValidMoves[i].x * 100 + 50);
        this->squareDirections.push_back(this->squareDirection);
    }
}


// Contructor, Destructor
Game::Game()
{
    this->initWindow();
    this->initBoard();
    this->initVariable();
    this->initSquareDirection();
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
                if (this->isTranform == true)
                {
                    cout << "OK" << endl;
                    if (this->board->board[x_end][y_end]->spritePiece.getGlobalBounds().contains(this->mousePosView))
                    {
                        if (this->mousePosView.x >= y_end * 100 && this->mousePosView.x < (y_end + 0.5) * 100
                            && this->mousePosView.y >= x_end * 100 && this->mousePosView.y < (x_end + 0.5) * 100)
                        {
                            this->isTranform = false;
                            delete this->board->board[x_end][y_end];

                            this->board->board[x_end][y_end] = PieceFactory::createPiece(ROOK * board->turn);
                            this->board->board[x_end][y_end]->spritePiece.setPosition(y_end * 100 + 50, x_end * 100 + 50);
                            this->isRender = true;
                            this->board->turn *= -1;
                            cout << this->board->turn << endl;
                            
                        }
                    }
                }
                else
                {
                    for (int i = 0; i < 8; i++)
                    {
                        for (int j = 0; j < 8; j++)
                        {
                            if (this->board->board[i][j] != nullptr && this->board->board[i][j]->spritePiece.getGlobalBounds().contains(this->mousePosView))
                            {
                                if (this->board->board[i][j]->getColor() == this->board->turn)
                                {
                                    this->isMove = true;
                                    this->x_start = i;
                                    this->y_start = j;

                                    // square Direction
                                    Point start;
                                    start.x = i;
                                    start.y = j;
                                    vector<Point> listValidMoves = this->board->board[i][j]->canMove(this->board->board, start);
                                    this->createSquareDirections(listValidMoves);
                                }
                            }
                        }
                    }
                }
            }
            break;
        case sf::Event::MouseButtonReleased:
            if (ev.key.code == sf::Mouse::Left)
            {
                cout << "true" << endl;
                this->isMove = false;

                // Set position in a square
                Point location = getPosition(this->mousePosView); // x y is different i j

                Point start;
                start.x = x_start;
                start.y = y_start;

                Point end;
                end.x = location.y;
                end.y = location.x;

                Piece* pieceStart = this->board->board[start.x][start.y];
                if (pieceStart == nullptr)
                    break;
                vector<Point> listValidMoves = pieceStart->canMove(this->board->board, start);
                if (this->board->canMoveInBoard(listValidMoves, end))
                {
                    //save moves
                    Moves mv(this->board->Get(end), start, end);
                    this->board->listMoves.push_back(mv);
                    this->board->numOfMoves++;

                    //swap positions
                    this->board->board[end.x][end.y] = this->board->board[start.x][start.y];
                    this->board->board[start.x][start.y] = nullptr;

                    //set sprite
                    this->board->board[end.x][end.y]->spritePiece.setPosition(location.x * 100 + 50, location.y * 100 + 50);

                    //is Pawn at the end of board
                    if (this->board->Get(end)->getType() == PAWN)
                    {
                        if ((end.X() == 7 && this->board->Get(end)->getColor() == -1) ||
                            (end.X() == 0 && this->board->Get(end)->getColor() == 1)) {

                            this->board->board[end.x][end.y]->transformSprite();
                            this->x_end = end.x;
                            this->y_end = end.y;
                            this->isTranform = true;
                        }
                    }

                    // square directions
                    this->squareDirections.erase(this->squareDirections.begin(), this->squareDirections.end());

                    //set turn
                    if(this->isTranform == false)
                        this->board->turn *= -1;
                    cout << this->board->turn << endl;

                    this->isRender = true;
                }
                else
                {
                    this->board->board[start.x][start.y]->spritePiece.setPosition(start.y * 100 + 50, start.x * 100 + 50); // return point start
                    this->isRender = true;
                }
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
    //cout << this->mousePosView.x << " " << this->mousePosView.y << endl;
}

void Game::updateMove()
{
    if (this->isMove)
    {
        this->board->board[x_start][y_start]->spritePiece.setPosition(this->mousePosView.x, this->mousePosView.y);
        this->isRender = true;
    }
}

void Game::updateTransform()
{
    /*if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && this->isTranform)
    {
        cout << "OK" << endl;
        if (this->board->board[x][y]->spritePiece.getGlobalBounds().contains(this->mousePosView))
        {
            if (this->mousePosView.x >= y * 100  && this->mousePosView.x < (y + 0.5) * 100
                && this->mousePosView.y >= x * 100 && this->mousePosView.y < (x + 0.5) * 100)
            {
                delete this->board->board[x][y];

                this->board->board[x][y] = PieceFactory::createPiece(ROOK * board->turn);
                this->board->board[x][y]->spritePiece.setPosition(y * 100 + 50, x * 100 + 50);
                this->isRender = true;
                this->board->turn *= -1;
                this->isTranform = false;
            }
        }
    }*/
}

void Game::update()
{
    // check zoom window
    if (this->window.getSize().x != 800 || this->window.getSize().y != 1200)
        this->isRender = true;

    this->updateMousePosition();

    this->pollEvents();

    this->updateMove();

    //this->updateTransform();
}

void Game::renderSquareDirecitons(sf::RenderTarget& target)
{
    for (int i = 0; i < this->squareDirections.size(); i++)
    {
        target.draw(this->squareDirections[i]);
    }
}


void Game::render()
{
    if (this->isRender)
    {
        // Clear
        this->window.clear(sf::Color::Black);

        // Draw
        this->board->renderBoard(this->window);

        // Draw square direction
        this->renderSquareDirecitons(this->window);

        // Display
        this->window.display();

        this->isRender = false;
    }
}
