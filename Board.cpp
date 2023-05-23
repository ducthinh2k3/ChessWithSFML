#include "Board.h"
#include "PieceFactory.h"

void Board::initBoard()
{
    int a[8][8] =
    { -1, -2, -3, -4, -5, -3, -2, -1,
     -6, -6, -6, -6, -6, -6, -6, -6,
     0, 0, 0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0, 0, 0,
     6, 6, 6, 6, 6, 6, 6, 6,
     1, 2, 3, 4, 5, 3, 2, 1 };
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            board[i][j] = PieceFactory::createPiece(a[i][j]);
            if (board[i][j] != nullptr)
                this->board[i][j]->setPosition(i, j);
        }
    }
}

Board::Board()
{
    this->initBoard();
    turn = 1; // 1: turn of white
    win = 0;
    this->isTransform = false;
    this->isRender = true;
    this->isMove = false;

    // Draw Board
    this->initTextureBoard();
    this->initSpriteBoard();

    // square directions
    this->initSquareDirection();
}


bool Board::canMoveInBoard(vector<Point> listValidMoves, Point end)
{
    for (int i = 0; i < listValidMoves.size(); i++)
    {
        if (listValidMoves[i] == end)
            return true;
    }

    return false;
}

bool Board::endGame()
{
    int cntPieceBlack = 0;
    int cntPieceWhite = 0;
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            if (board[i][j] != nullptr)
            {
                if (board[i][j]->getColor() * board[i][j]->getType() < 0)
                    cntPieceBlack++;
                else if (board[i][j]->getColor() * board[i][j]->getType() > 0)
                    cntPieceWhite++;
            }
        }
    }

    if (cntPieceBlack != 0 && cntPieceWhite == 0)
    {
        win = -1;
        return true;
    }
    else if (cntPieceBlack == 0 && cntPieceWhite != 0)
    {
        win = 1;
        return true;
    }

    return false;
}

Board::~Board()
{
    for (int i = 0; i < listMoves.size(); i++)
    {
        if (listMoves[i].Dich != nullptr)
            delete listMoves[i].Dich;
    }

    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            delete board[i][j];
        }
    }
}

void Board::clearBoard()
{
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            delete board[i][j];
            board[i][j] = NULL;
        }
    }
}

Piece*& Board::Get(Point a)
{
    return board[a.X()][a.Y()];
}

// Graphic
void Board::transformPawn(sf::Vector2f mousePosView)
{
    cout << "OK" << endl;
    if (this->board[end.x][end.y]->spritePiece.getGlobalBounds().contains(mousePosView))
    {
        this->isTransform = false;
        delete this->board[end.x][end.y];

        if (mousePosView.x >= end.y * 100 && mousePosView.x < (end.y + 0.5) * 100 
            && mousePosView.y >= end.x * 100 && mousePosView.y < (end.x + 0.5) * 100)
        {
            this->board[end.x][end.y] = PieceFactory::createPiece(ROOK * turn);
        }
        else if (mousePosView.x >= (end.y + 0.5) * 100 && mousePosView.x <= (end.y + 1) * 100
            && mousePosView.y >= end.x * 100 && mousePosView.y < (end.x + 0.5) * 100)
        {
            this->board[end.x][end.y] = PieceFactory::createPiece(KING * turn);
        }
        else if (mousePosView.x >= end.y * 100 && mousePosView.x < (end.y + 0.5) * 100
            && mousePosView.y >= (end.x + 0.5) * 100 && mousePosView.y <= (end.x + 1) * 100)
        {
            this->board[end.x][end.y] = PieceFactory::createPiece(BISHOP * turn);
        }
        else if (mousePosView.x >= (end.y + 0.5) * 100 && mousePosView.x <= (end.y + 1) * 100
            && mousePosView.y >= (end.x + 0.5) * 100 && mousePosView.y <= (end.x + 1) * 100)
        {
            this->board[end.x][end.y] = PieceFactory::createPiece(KNIGHT * turn);
        }

        
        this->board[end.x][end.y]->spritePiece.setPosition(end.y * 100 + 50, end.x * 100 + 50);
        this->turn *= -1;
        this->isRender = true;
        cout << this->turn << endl;
    }
}

void Board::savePointStart(sf::Vector2f mousePosView)
{
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            if (this->board[i][j] != nullptr && this->board[i][j]->spritePiece.getGlobalBounds().contains(mousePosView))
            {
                if (this->board[i][j]->getColor() == this->turn)
                {
                    this->isMove = true;
                    this->start.x = i;
                    this->start.y = j;

                    // square Direction
                    vector<Point> listValidMoves = this->board[i][j]->canMove(this->board, start);
                    this->createSquareDirections(listValidMoves);
                }
            }
        }
    }
}

Point Board::getPositionPiece(sf::Vector2f mousePosView)
{
    Point location;
    location.x = (int)mousePosView.x / 100;
    location.y = (int)mousePosView.y / 100;
    return location;
}

void Board::Move(sf::Vector2f mousePosView)
{
    cout << "true" << endl;
    this->isMove = false;

    Point location = getPositionPiece(mousePosView); // x y is different i j
    if (this->isTransform == false)
    {
        this->end.x = location.y;
        this->end.y = location.x;
    }

    Piece* pieceStart = this->board[start.x][start.y];
    if (pieceStart == nullptr)
        return;
    vector<Point> listValidMoves = pieceStart->canMove(this->board, start);

    if (this->canMoveInBoard(listValidMoves, end))
    {
        // save moves
        Moves mv(this->Get(end), start, end);
        this->listMoves.push_back(mv);
        this->numOfMoves++;

        // swap positions
        this->board[end.x][end.y] = this->board[start.x][start.y];
        this->board[start.x][start.y] = nullptr;

        // set sprite
        this->board[end.x][end.y]->spritePiece.setPosition(location.x * 100 + 50, location.y * 100 + 50);

        // is Pawn at the end of board
        if (this->Get(end)->getType() == PAWN)
        {
            if ((end.X() == 7 && this->Get(end)->getColor() == -1) ||
                (end.X() == 0 && this->Get(end)->getColor() == 1))
            {

                this->board[end.x][end.y]->transformSprite();
                this->isTransform = true;
            }
        }

        // square directions
        this->squareDirections.erase(this->squareDirections.begin(), this->squareDirections.end());

        // set turn
        if (this->isTransform == false)
            this->turn *= -1;
        cout << this->turn << endl;

        this->isRender = true;
    }
    else
    {
        this->board[start.x][start.y]->spritePiece.setPosition(start.y * 100 + 50, start.x * 100 + 50); // return point start
        this->isRender = true;
    }
}

void Board::updateMove(sf::Vector2f mousePosView)
{
    if (this->isMove)
    {
        this->board[start.x][start.y]->spritePiece.setPosition(mousePosView.x, mousePosView.y);
        this->isRender = true;
    }
}

// Square Direction
void Board::initSquareDirection()
{
    this->squareDirection.setRadius(10);
    this->squareDirection.setFillColor(sf::Color(192, 192, 192));
    this->squareDirection.setOrigin(squareDirection.getRadius(), squareDirection.getRadius());
}

void Board::createSquareDirections(vector<Point> listValidMoves)
{
    this->squareDirections.erase(this->squareDirections.begin(), this->squareDirections.end());
    for (int i = 0; i < listValidMoves.size(); i++)
    {
        this->squareDirection.setPosition(listValidMoves[i].y * 100 + 50, listValidMoves[i].x * 100 + 50);
        this->squareDirections.push_back(this->squareDirection);
    }
}

// Draw Board
void Board::initTextureBoard()
{
    if (!this->textureBoard.loadFromFile("Picture/Board.png"))
    {
        std::cout << "ERROR::PLAYER::INITTEXTURE::Could not load texture file."
            << "\n";
    }
}

void Board::initSpriteBoard()
{
    this->spriteBoard.setTexture(this->textureBoard);
}

void Board::renderBoard(sf::RenderTarget& target)
{
    target.draw(this->spriteBoard);

    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            if (board[i][j] != nullptr)
            {
                board[i][j]->renderPiece(target);
            }
        }
    }
}

// Draw Square Direction
void Board::renderSquareDirecitons(sf::RenderTarget& target)
{
    for (int i = 0; i < this->squareDirections.size(); i++)
    {
        target.draw(this->squareDirections[i]);
    }
}