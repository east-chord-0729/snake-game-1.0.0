#pragma once

#include "Board.hpp"
#include "GameController.hpp"
#include "ObjectGate.hpp"
#include "ObjectItems.hpp"
#include "ObjectSnake.hpp"
#include "ObjectWalls.hpp"
#include "BoardScore.hpp"
#include "BoardMission.hpp"
#include <chrono>
#include <iostream>

using namespace std;

class Game
{
  public:
    Game();
    ~Game();
    void run();

  private:
    GameController gameController;
    Board board;
    BoardScore boardScore;
    BoardMission boardMission;
    Snake snake;
    GoodItem goodItem;
    BadItem badItem;
    SlowItem slowItem;
    Gate gate1, gate2; // GATE 두개 추가
    int gameSpeed, gameLevel, gameTick;
    bool gameOver;
    int snakeLength = 0;
    int gateLength = 0;

    void handleInput();
    void updateState();
    void render();
    void displayGameStart();
    void displayGameOver();
    void GateDirection(SnakeSegment gateHead); // GATE 방향 설정 //! 삭제 예정.
};

Game::Game()
{
    /* Init ncurses and srand */
    initscr();             /* Start ncurses */
    clear();               /* Clear screen */
    noecho();              /* Don't echo user input */
    cbreak();              /* Enable immediate input */
    curs_set(0);           /* Hide cursor */
    keypad(stdscr, TRUE);  /* Enable special keys */
    nodelay(stdscr, TRUE); /* Non-blocking input */
    srand(time(0));        /* Seed random number generator */

    /* Init member variables */
    board = Board(BOARD_COLS, BOARD_ROWS);
    gameSpeed = 300;
    gameLevel = 1; //! 0으로 설정하게
    gameOver = false;
    gameTick = 1;

    /* Generate objects */
    gameController.generateWalls(board, gameLevel);
    gameController.generateGates(board, gate1, gate2);
    gameController.generateSnake(board, snake);
    gameController.generateItem(board, goodItem);
    gameController.generateItem(board, badItem);
    gameController.generateItem(board, slowItem);
}

Game::~Game()
{
    endwin();
}

void Game::run()
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
}

void Game::handleInput()
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

void Game::updateState()
{
    /* freeze the game temporarily to controll game speed */
    napms(gameSpeed);

    /* re-generate the items after 5 seconds */
    if (gameTick++ % 100 == 0)
    {
        gameController.removeGate(board, gate1, gate2);
        gameController.removeItem(board, goodItem);
        gameController.removeItem(board, badItem);
        gameController.removeItem(board, slowItem);
        gameController.generateItem(board, goodItem);
        gameController.generateItem(board, badItem);
        gameController.generateItem(board, slowItem);
        gameController.generateGates(board, gate1, gate2);
    }

    gameController.checkGates(board, gate1, gate2, gateLength, snakeLength);

    SnakeSegment nextHead = snake.getNextHead(); //! 삭제 예정.

    /* check collision */
    chtype collisionIcon = board.getIcon(nextHead.getY(), nextHead.getX());
    switch (collisionIcon)
    {
    case ICON_EMPTY:
        gameController.moveSnake(board, snake);
        break;
    case ICON_ITEM_GOOD:
        gameController.eatGoodItemAndMove(board, snake);
        break;
    case ICON_ITEM_BAD:
        gameController.eatBadItemAndMove(board, snake);
        break;
    case ICON_ITEM_SLOW:
        gameSpeed -= 50;
        gameController.moveSnake(board, snake);
        break;
    case ICON_SNAKE:
        gameOver = true;
        break;
    case ICON_WALL:
        gameOver = true;
        break;
    case ICON_IWALL:
        gameOver = true;
        break;
    case ICON_GATE:
        gameController.passGate(board, snake, gate1, gate2);
        gameController.moveSnake(board, snake);
        snakeLength = snake.getLength();
        break;
    default:
        break;
    }

    /* check snake state */
    if (snake.getLength() < 3)
    {
        gameOver = true;
    }
}

void Game::render()
{
    board.render();
}

void Game::displayGameStart()
{
    ; // TODO: 'Enter 누르면 게임 시작, 다른 키 누르면 종료' 라는 알림
}

void Game::displayGameOver()
{
    //! 화면 싹 지우고 중심에 글 뜨게 하기
    //! 잠시후에 사라지지 말고, 버튼을 눌러야 사라지기.
    if (gameOver)
    {
        mvprintw(0, 0, "Game Over! press any key to exit");
        mvprintw(1, 0, "Your snake length: %d", snake.getLength());
        refresh();
        napms(2000);
    }
}