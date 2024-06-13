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
    gameLevel = 3; //! 0으로 설정하게
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
    if (gameTick++ % 10 == 0)
    {
        gameController.removeItem(board, goodItem);
        gameController.removeItem(board, badItem);
        gameController.removeItem(board, slowItem);
        gameController.generateItem(board, goodItem);
        gameController.generateItem(board, badItem);
        gameController.generateItem(board, slowItem);
        //! remove gate, generate gate. 이거 최우선 구현.
    }

    SnakeSegment nextHead = snake.getNextHead(); //! 삭제 예정.
    SnakeSegment gateHead = SnakeSegment(0, 0); // 또다른 GATE가 들어갈 변수 //! 삭제 예정

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
        //! 이거 Controller로 옮기기.
        if (nextHead.getY() == gate1.getY() && nextHead.getX() == gate1.getX())
        {                                                        // GATE1에 닿았을 때
            gateHead = SnakeSegment(gate2.getY(), gate2.getX()); // GATE2를 gateHead에 저장
        }
        else
        {                                                        // GATE2에 닿았을 때
            gateHead = SnakeSegment(gate1.getY(), gate1.getX()); // GATE1을 gateHead에 저장
        }
        snake.addHead(gateHead); // gateHead를 snake의 head로 설정
        board.removeObject(snake.getTail());
        snake.removeTail();

        GateDirection(gateHead); // GATE 방향 설정
        gameController.moveSnake(board, snake);
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

//! 이것도 뭔가 여기있으면 안됨.
void Game::GateDirection(SnakeSegment gateHead)
{
    SnakeSegment testnextHead = snake.getNextHead(); // 원래 방향으로 이동했을 때의 위치
    Object testcollisionObject = board.getObject(testnextHead.getX(), testnextHead.getY());

    if (gateHead.getY() == 0)
    {                             // GATE가 맵의 상단에 있을떄
        snake.setDirection(DOWN); // GATE의 방향을 DOWN으로 설정
    }
    else if (gateHead.getY() == BOARD_COLS - 1)
    {                           // GATE가 맵의 하단에 있을때
        snake.setDirection(UP); // GATE의 방향을 UP으로 설정
    }
    else if (gateHead.getX() == 0)
    {                              // GATE가 맵의 좌측에 있을때
        snake.setDirection(RIGHT); // GATE의 방향을 RIGHT으로 설정
    }
    else if (gateHead.getX() == BOARD_ROWS - 1)
    {                             // GATE가 맵의 우측에 있을때
        snake.setDirection(LEFT); // GATE의 방향을 LEFT으로 설정
    }
    else
    { // GATE가 맵의 가장자리에 있지 않을때
        // 원래의 방향으로 갔을 떄 벽이 있을때만 방향을 바꿈
        if (testcollisionObject.getIcon() == ICON_WALL)
        { // 원래 방향으로 이동했을때 벽이 있을때
            switch (snake.getDirection())
            {
            case UP:                       // 원래 방향이 UP일때
                snake.setDirection(RIGHT); // GATE의 방향을 RIGHT으로 설정
                break;
            case DOWN:                    // 원래 방향이 DOWN일때
                snake.setDirection(LEFT); // GATE의 방향을 LEFT으로 설정
                break;
            case LEFT:                  // 원래 방향이 LEFT일때
                snake.setDirection(UP); // GATE의 방향을 UP으로 설정
                break;
            case RIGHT:                   // 원래 방향이 RIGHT일때
                snake.setDirection(DOWN); // GATE의 방향을 DOWN으로 설정
                break;
            default:
                break;
            }
        }
    }
}