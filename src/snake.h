#pragma once

#include <ncurses.h>
#include <queue>
#include <stdlib.h>
#include <time.h>

#define BOARD_DIM 17
#define BOARD_ROWS BOARD_DIM
#define BOARD_COLS BOARD_DIM * 2

enum Direction
{
    UP = -1,
    DOWN = 1,
    LEFT = -2,
    RIGHT = 2
};

class Drawable
{
  protected:
    int y, x;
    chtype icon;

  public:
    Drawable();
    Drawable(int y, int x, chtype ch);
    int getY();
    int getX();
    chtype getIcon();
};

class Board
{
  private:
    int height, width;
    WINDOW *boardWin;
    void construct(int height, int width);

  public:
    Board();
    Board(int height, int width);
    void initialize();
    void addBorder();
    void add(Drawable drawable);
    void addAt(int y, int x, chtype ch);
    chtype getInput();
    chtype getCharAt(int y, int x);
    void getEmptyCoordinates(int &y, int &x);
    void clear();
    void refresh();
    void setTimeout(int timeout);
};

class Apple : public Drawable
{
  public:
    Apple(int y = 0, int x = 0);
};

class Empty : public Drawable
{
  public:
    Empty(int y = 0, int x = 0);
};

class SnakePiece : public Drawable
{
  public:
    SnakePiece(int y = 0, int x = 0);
};

class Snake
{
  private:
    std::queue<SnakePiece> prevPieces;
    Direction currDirection;

  public:
    Snake();
    void addPiece(SnakePiece piece);
    void removePiece();
    SnakePiece tail();
    SnakePiece head();
    Direction getDirection();
    void setDirection(Direction d);
    SnakePiece nextHead();
};

class SnakeGame
{
  private:
    Board board;
    bool gameOver;
    Apple *apple;
    Snake snake;
    void handleNextPiece(SnakePiece);
    void createApple();
    void destroyApple();

  public:
    SnakeGame(int height = BOARD_ROWS, int width = BOARD_COLS);
    ~SnakeGame();
    void initialize();
    void processInput();
    void updateState();
    void redraw();
    bool isOver();
};