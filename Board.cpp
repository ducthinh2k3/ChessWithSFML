#include "Board.h"
#include "PieceFactory.h"

void Board::initBoard()
{
     int a[8][8]= 
         {-1,-2,-3,-4,-5,-3,-2,-1,
         -6,-6,-6,-6,-6,-6,-6,-6,
         0, 0, 0, 0, 0, 0, 0, 0,
         0, 0, 0, 0, 0, 0, 0, 0,
         0, 0, 0, 0, 0, 0, 0, 0,
         0, 0, 0, 0, 0, 0, 0, 0,
         6, 6, 6, 6, 6, 6, 6, 6,
         1, 2, 3, 4, 5, 3, 2, 1};
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
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

    // Draw Board
    this->initTextureBoard();
    this->initSpriteBoard();
}

vector<Point> Board::renderDirecionMove(Point start)
{
    Piece* pieceStart = board[start.x][start.y];
    vector<Point> listValidMoves = pieceStart->canMove(board, start);

    int a[8][8];
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            a[i][j] = 0;
        }
    }

    if (pieceStart != nullptr)
        a[start.x][start.y] = pieceStart->getColor() * pieceStart->getType();

    for (int i = 0; i < listValidMoves.size(); i++)
    {
        Point temp = listValidMoves[i];
        a[temp.x][temp.y] = 9;
    }

    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            cout << a[i][j] << "  ";
        }
        cout << "| " << i << endl;
    }
    cout << "------------------------\n";
    cout << "0  1  2  3  4  5  6  7" << endl;
    return listValidMoves;
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

//void Board::Move()
//{
//    Point start;
//    do
//    {
//        cout << "Input point start: ";
//        cin >> start.x >> start.y;
//    } while ((start.x > 7 || start.x < 0) || (start.y > 7 || start.y < 0));
//
//    if (board[start.x][start.y] != nullptr && board[start.x][start.y]->getColor() == turn) // check turn is right
//    {
//        cout << endl
//            << "Direction move of Piece you choose" << endl;
//        vector<Point> listValidMoves = renderDirecionMove(start);
//        cout << endl;
//
//        Point end;
//        do
//        {
//            cout << "Input point end: ";
//            cin >> end.x >> end.y;
//        } while ((end.x > 7 || end.x < 0) || (end.y > 7 || end.y < 0));
//
//        if (canMoveInBoard(listValidMoves, end))
//        {
//
//            //save moves
//            Moves mv(Get(end), start, end);
//            listMoves.push_back(mv);
//            numOfMoves++;
//
//            //swap positions
//            board[end.x][end.y] = board[start.x][start.y];
//            board[start.x][start.y] = nullptr;
//
//            //is Pawn at the end of board
//            if (Get(end)->getType() == PAWN)
//            {
//                if ((end.X() == 7 && Get(end)->getColor() == -1) ||
//                    (end.X() == 0 && Get(end)->getColor() == 1)) {
//                    // change title
//                    renderBoard();
//                    cout << "1. ROOK  " << endl;
//                    cout << "2. KNIGHT" << endl;
//                    cout << "3. BISHOP" << endl;
//                    cout << "4. QUEEN" << endl;
//                    int type = 4;
//                    cout << "Choose the title for Pawn: ";
//                    cin >> type;
//                    while (type < 1 || type > 4) {
//                        cout << "Title for Pawn(1-4): ";
//                        cin >> type;
//                    }
//                    type *= turn;
//                    delete board[end.X()][end.Y()];
//
//                    board[end.X()][end.Y()] = PieceFactory::createPiece(type);
//                    Moves mv2(Get(end), end, end);
//                    listMoves.push_back(mv2);
//                    numOfMoves++;
//                }
//            }
//            turn *= -1;
//        }
//    }
//}

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

void Board::renderBoard()
{
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            if (board[i][j] != nullptr)
            {
                cout << board[i][j]->getColor() * board[i][j]->getType() << "   ";
            }
            else
                cout << "0   ";
        }
        cout << endl;
    }
}

void Board::Play()
{
    while (!endGame())
    {
        cout << "Turn: " << turn << endl;
        renderBoard();
        //Move();
    }
    if (win == 1)
        cout << "White win " << endl;
    else if (win == -1)
        cout << "Black win " << endl;
}

Board::~Board()
{
    for (int i = 0; i < listMoves.size(); i++)
    {
        if(listMoves[i].Dich != nullptr)
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

Piece*& Board::Get(Point a) {
    return board[a.X()][a.Y()];
}

// Draw Board
void Board::initTextureBoard()
{
    if (!this->textureBoard.loadFromFile("Picture/Board.png"))
    {
        std::cout << "ERROR::PLAYER::INITTEXTURE::Could not load texture file." << "\n";
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