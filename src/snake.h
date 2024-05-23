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
    Board();                                  //
    Board(int height, int width);             //
    void initialize();                        // 중요
    void addBorder();                         //
    void add(Drawable drawable);              // 이게 중요.
    void addAt(int y, int x, chtype ch);      // 사실 안씀.
    chtype getInput();                        //
    chtype getCharAt(int y, int x);           // 중요
    void getEmptyCoordinates(int &y, int &x); // 동현만 중요할지도
    void clear();                             //
    void refresh();                           //
    void setTimeout(int timeout);             //
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

class Wall : public Drawable
{
};

class Snake
{
  private:
    std::queue<SnakePiece> prevPieces; // SnakePiece[] pieces;
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

/*

  상속은 is 관계

  정사각형은 도형이다.
  원은 도형이지.
  삼각형도 도형이야.

  정사각형 원 삼각형 --상속-- 도형

  스네이크전체는 스네이크부분인가? 아님 이건 have

*/