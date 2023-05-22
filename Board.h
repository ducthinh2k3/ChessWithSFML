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

    //Draw board
    void initTextureBoard();
    void initSpriteBoard();
    sf::Texture textureBoard;
    sf::Sprite spriteBoard;

public:
    Board();
    void initBoard();
    bool canMoveInBoard(vector<Point> listValidMoves, Point end);
    //void Move();
    bool endGame();
    void renderBoard();
    void Play();
    vector<Point> renderDirecionMove(Point start);
    void clearBoard();
    Piece*& Get(Point a);
    ~Board();

    //Draw board
    void renderBoard(sf::RenderTarget& target);
};

