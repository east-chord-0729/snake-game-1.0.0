#pragma once

#include "Board.hpp"
#include "ObjectItems.hpp"
#include "ObjectSnake.hpp"
#include "ObjectWalls.hpp"
#include <ncurses.h>

class SnakeGame
{
  public:
    SnakeGame();
    ~SnakeGame();
    void run();

  private:
    Board board;
    Snake snake;
    GoodItem goodItem;
    BadItem badItem;
    int gameSpeed;
    bool gameOver;

    void handleInput();
    void updateState();
    void render();
    void displayGameStart();
    void displayGameOver();
};

SnakeGame::SnakeGame()
{
    initscr();             /* ncurses 화면 초기화 */
    clear();               /* 화면 지우기 */
    noecho();              /* 입력된 문자를 화면에 표시하지 않음 */
    cbreak();              /* 줄 버퍼링을 사용하지 않음 */
    curs_set(0);           /* 기본 커서를 숨김 */
    keypad(stdscr, TRUE);  /* 함수키(방향키 등) 입력을 가능하게 함 */
    nodelay(stdscr, TRUE); /* 입력 대기 없이 getch가 즉시 반환되도록 설정 */
    srand(time(0));

    board = Board(13, 25);
    gameSpeed = 500;
    gameOver = false;

    for(int i=1;i<24;i++){
        board.addObject(Wall(0, i));
        board.addObject(Wall(12, i));
    }
    for(int i=1;i<12;i++){
        board.addObject(Wall(i, 0));
        board.addObject(Wall(i, 24));
    }

    board.addObject(ImmuneWall(0,0));
    board.addObject(ImmuneWall(12,0));
    board.addObject(ImmuneWall(0,24));
    board.addObject(ImmuneWall(12,24));

    goodItem = GoodItem(8, 8);
    badItem = BadItem(4, 12);
    board.addObject(goodItem);
    board.addObject(badItem);

    snake = Snake(3, 5, RIGHT);
    board.addObject(snake.getHead());
    snake.addHead(snake.getNextHead());
    board.addObject(snake.getHead());
    snake.addHead(snake.getNextHead());
    board.addObject(snake.getHead()); // TODO snake가 이미 길게 생성된 상태에서 한 번에 보드에 추가하고 싶음.
}

SnakeGame::~SnakeGame()
{
    endwin();
}

void SnakeGame::run()
{
    displayGameStart();

    // TODO: 키 입력 받기.

    while (!gameOver)
    {
        handleInput();
        updateState();
        render();
    }

    displayGameOver();

    // TODO: 키 입력 받기.
}

void SnakeGame::handleInput()
{
    int ch = getch();

    switch (ch)
    {
    case KEY_UP:
        snake.setDirection(UP);
        break;
    case KEY_DOWN:
        snake.setDirection(DOWN);
        break;
    case KEY_LEFT:
        snake.setDirection(LEFT);
        break;
    case KEY_RIGHT:
        snake.setDirection(RIGHT);
        break;
    case 'q':
        gameOver = true;
        break;
    }
}

void SnakeGame::updateState()
{
    SnakeSegment nextHead = snake.getNextHead();
    Object collisionObject = board.getObject(nextHead.getX(), nextHead.getY());

    /* check collision */
    switch (collisionObject.getIcon())
    {
    case ICON_EMPTY:
        board.addObject(nextHead);
        snake.addHead(nextHead);
        board.removeObject(snake.getTail());
        snake.removeTail();
        return;
    case ICON_ITEM_GOOD:
        board.addObject(nextHead);
        snake.addHead(nextHead);
        return;
    case ICON_ITEM_BAD:
        board.removeObject(snake.getTail());
        snake.removeTail();
        board.addObject(nextHead);
        snake.addHead(nextHead);
        board.removeObject(snake.getTail());
        snake.removeTail();
        return;
    case ICON_ITEM_SLOW:
        gameSpeed -= 50;
        board.addObject(nextHead);
        snake.addHead(nextHead);
        board.removeObject(snake.getTail());
        snake.removeTail();
        return;
    case ICON_SNAKE:
        gameOver = true;
        return;
    case ICON_WALL:
        gameOver = true;
        return;
    case ICON_IWALL:
        gameOver = true;
        return;
    case ICON_GATE:
        //! Gate 구현하기
        return;
    default:
        break;
    }
}

void SnakeGame::render()
{
    board.render();
    napms(gameSpeed);
}

void SnakeGame::displayGameStart()
{
    ; // TODO: 'Enter 누르면 게임 시작, 다른 키 누르면 종료' 라는 알림
}

void SnakeGame::displayGameOver()
{
    ; // TODO: '아무 키나 누르면 종료'라는 알림
}