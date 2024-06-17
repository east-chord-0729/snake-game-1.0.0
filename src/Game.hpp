#pragma once

#include "Board.hpp"
#include "BoardMission.hpp"
#include "BoardScore.hpp"
#include "GameController.hpp"
#include "ObjectGate.hpp"
#include "ObjectItems.hpp"
#include "ObjectSnake.hpp"
#include "ObjectWalls.hpp"
#include <chrono>
#include <iostream>

using namespace std::chrono;
using namespace std;

steady_clock::time_point start_time;

class Game
{
  public:
    Game(int level = 0);
    ~Game();
    bool run();

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
    int countGoodItem, countBadItem, countSlowItem, countGate;
    int gateLength, snakeLength, gameSpeed, gameTick;
    bool gameOver, nextLevel;

    void handleInput();
    void updateState();
    void render();
};

Game::Game(int level)
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
    boardScore = BoardScore(BOARD_COLS, BOARD_ROWS);
    boardMission = BoardMission(BOARD_COLS, BOARD_ROWS);

    /* Init values */
    gameSpeed = 500;
    gameOver = false;
    gameTick = 1;
    countGoodItem = 0;
    countSlowItem = 0;
    countBadItem = 0;
    countGate = 0;
    snakeLength = 0;
    gateLength = 0;
    nextLevel = false;

    /* Generate objects */
    gameController.generateWalls(board, level);
    gameController.generateGates(board, gate1, gate2);
    gameController.generateSnake(board, snake);
    gameController.generateItem(board, goodItem);
    gameController.generateItem(board, badItem);
    gameController.generateItem(board, slowItem);

    /* Init scores */
    boardScore.updateScore(snake.getLength(), 0);
    boardScore.updateScore(countGoodItem, 1);
    boardScore.updateScore(countBadItem, 2);
    boardScore.updateScore(countSlowItem, 3);
    boardScore.updateScore(countGate, 4);
}

Game::~Game()
{
    endwin();
}

bool Game::run()
{
    start_time = steady_clock::now();

    while (!gameOver)
    {
        handleInput();
        updateState();
        render();
    }

    return nextLevel;
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

    /* timer */
    steady_clock::time_point current_time = steady_clock::now();
    duration<double> elapsed_seconds = duration_cast<duration<double>>(current_time - start_time);
    mvprintw(0, 0, "Elapsed Time: %.f seconds", elapsed_seconds.count());

    /* re-generate the items after 5 seconds */
    if (gameTick++ % 50 == 0)
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

    SnakeSegment nextHead = snake.getNextHead();
    chtype collisionIcon = board.getIcon(nextHead.getY(), nextHead.getX());

    /* check collision */
    switch (collisionIcon)
    {
    case ICON_EMPTY:
        gameController.moveSnake(board, snake);
        break;
    case ICON_ITEM_GOOD:
        gameController.eatGoodItemAndMove(board, snake);
        boardScore.updateScore(++countGoodItem, 1);
        boardScore.updateScore(snake.getLength(), 0);
        boardMission.updateMisson(countGoodItem, 1);
        boardMission.updateMisson(snake.getLength(), 0);
        break;
    case ICON_ITEM_BAD:
        gameController.eatBadItemAndMove(board, snake);
        boardScore.updateScore(++countBadItem, 2);
        boardScore.updateScore(snake.getLength(), 0);
        boardMission.updateMisson(countBadItem, 2);
        boardMission.updateMisson(snake.getLength(), 0);
        break;
    case ICON_ITEM_SLOW:
        gameSpeed -= 50;
        gameController.moveSnake(board, snake);
        boardScore.updateScore(++countSlowItem, 3);
        boardMission.updateMisson(countSlowItem, 3);
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
        boardScore.updateScore(++countGate, 4);
        boardMission.updateMisson(countGate, 4);
        break;
    default:
        break;
    }

    /* check snake state */
    if (snake.getLength() < 3)
    {
        gameOver = true;
    }

    /* mission completes */
    if (snake.getLength() == 5 && countGate == 1 && countSlowItem == 1 && countGoodItem == 3 && countBadItem == 1)
    {
        gameOver = true;
        nextLevel = true;
    }
}

void Game::render()
{
    board.render();
    boardScore.render();
    boardMission.render();
}