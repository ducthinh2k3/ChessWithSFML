#pragma once

#include "Queen.h"
#include "Knight.h"
#include "Pawn.h"
#include "King.h"
#include "Bishop.h"
#include "Rook.h"
#include "Moves.h"

class Board
{
public:
    Piece* board[8][8];
    int turn;
    int win; // 1: White win, -1: Black win
    vector<Moves> listMoves;
    int numOfMoves;

    // Draw board
    void initTextureBoard();
    void initSpriteBoard();
    sf::Texture textureBoard;
    sf::Sprite spriteBoard;

    //Graphic
    bool isTransform;
    bool isRender;
    bool isMove;
    Point start;
    Point end;

    // Square direction
    sf::CircleShape squareDirection;
    vector<sf::CircleShape> squareDirections;

public:
    Board();
    void initBoard();
    bool canMoveInBoard(vector<Point> listValidMoves, Point end);
    bool endGame();
    void clearBoard();
    Piece*& Get(Point a);

    ~Board();

    // Draw board
    void renderBoard(sf::RenderTarget& target);

    // Graphics
    void transformPawn(sf::Vector2f mousePosView);
    void savePointStart(sf::Vector2f mousePosView);
    Point getPositionPiece(sf::Vector2f mousePosView);
    void Move(sf::Vector2f mousePosView);
    void updateMove(sf::Vector2f mousePosView);

    // square Direction
    void initSquareDirection();
    void createSquareDirections(vector<Point> listValidMoves);
    void renderSquareDirecitons(sf::RenderTarget& target);
};
