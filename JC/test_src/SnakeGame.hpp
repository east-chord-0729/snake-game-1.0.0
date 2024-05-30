#pragma once

#include "Board.hpp"
#include "ObjectItems.hpp"
#include "ObjectSnake.hpp"
#include "ObjectWalls.hpp"
#include "ObjectGate.hpp"
#include <ncurses.h>
#include <iostream>

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
    Gate* gate1;
    Gate* gate2;
    int gameSpeed;
    bool gameOver;
    int snakeLength=0;
    int gateLength=0;

    void handleInput();
    void updateState();
    void render();
    void displayGameStart();
    void displayGameOver();
    void generateGate();
    void GateDirection(SnakeSegment gatehead);
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

    board = Board(BOARD_COLS, BOARD_ROWS);
    gameSpeed = 500;
    gameOver = false;

    // for(int i=1;i<BOARD_ROWS-1;i++){
    //     board.addObject(Wall(0, i));
    //     board.addObject(Wall(BOARD_COLS-1, i));
    // }
    // for(int i=1;i<BOARD_COLS-1;i++){
    //     board.addObject(Wall(i, 0));
    //     board.addObject(Wall(i, BOARD_ROWS-1));
    // }

    // board.addObject(ImmuneWall(0,0));
    // board.addObject(ImmuneWall(BOARD_COLS-1,0));
    // board.addObject(ImmuneWall(0,BOARD_ROWS-1));
    // board.addObject(ImmuneWall(BOARD_COLS-1,BOARD_ROWS-1));
    int mapnum = 1;
    for(int i=0;i<BOARD_COLS;i++){
        for(int j=0;j<BOARD_ROWS;j++){
            if(map[mapnum][i][j] == 1){
                board.addObject(Wall(i,j));
            }
            else if(map[mapnum][i][j] == 2){
                board.addObject(ImmuneWall(i,j));
            }
            else if(map[mapnum][i][j] == 3){
                board.addObject(GoodItem(i,j));
            }
            else if(map[mapnum][i][j] == 4){
                board.addObject(BadItem(i,j));
            }
        }
    }

    // goodItem = GoodItem(8, 8);
    // badItem = BadItem(4, 12);
    // board.addObject(goodItem);
    // board.addObject(badItem);

    snake = Snake(3, 5, RIGHT);
    board.addObject(snake.getHead());
    snake.addHead(snake.getNextHead());
    board.addObject(snake.getHead());
    snake.addHead(snake.getNextHead());
    board.addObject(snake.getHead()); // TODO snake가 이미 길게 생성된 상태에서 한 번에 보드에 추가하고 싶음.

    gate1 = NULL;
    gate2 = NULL;
    generateGate();
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

void SnakeGame::generateGate()
{
    std::vector<Wall> wallPositions;
    for (int y = 0; y < BOARD_ROWS; y++) {
        for (int x = 0; x < BOARD_COLS; x++) {
            Object obj = board.getObject(x, y);
            if (obj.getIcon() == ICON_WALL) {
                wallPositions.push_back(Wall(y, x));
            }
        }
    }

    std::random_shuffle(wallPositions.begin(), wallPositions.end());

    if (wallPositions.size() >= 2) {
        Wall pos1 = wallPositions[0];
        Wall pos2 = wallPositions[1];

        gate1 = new Gate(pos1.getY(), pos1.getX());
        gate2 = new Gate(pos2.getY(), pos2.getX());
        gate1->pairWith(gate2);

        board.addObject(*gate1);
        board.addObject(*gate2);
    }
}

void SnakeGame::updateState()
{
    SnakeSegment nextHead = snake.getNextHead();
    Object collisionObject = board.getObject(nextHead.getX(), nextHead.getY());
    SnakeSegment testnextHead = SnakeSegment(0, 0);

    SnakeSegment gatehead = SnakeSegment(0, 0);
    
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
        if (nextHead.getY() == gate1->getY() && nextHead.getX() == gate1->getX()) {
            gatehead = SnakeSegment(gate2->getY(), gate2->getX());
        }
        else {
            gatehead = SnakeSegment(gate1->getY(), gate1->getX());
        }
        snake.addHead(gatehead);
        board.removeObject(snake.getTail());
        snake.removeTail();
        GateDirection(gatehead);
        testnextHead = snake.getNextHead();
        board.addObject(testnextHead);
        snake.addHead(testnextHead);
        board.removeObject(snake.getTail());
        snake.removeTail();
        board.addObject(Wall(nextHead.getY(), nextHead.getX()));
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
    if(gameOver){
        mvprintw(0, 0, "Game Over! press any key to exit");
        // snake의 길이 출력
        mvprintw(1, 0, "Your snake length: %d", snake.getLength());
        refresh();
        napms(2000);
    }
}

void SnakeGame::GateDirection(SnakeSegment gatehead)
{
    if(gatehead.getY() == 0){
        snake.setDirection(DOWN);
    }
    else if(gatehead.getY() == BOARD_COLS-1){
        snake.setDirection(UP);
    }
    else if(gatehead.getX() == 0){
        snake.setDirection(RIGHT);
    }
    else if(gatehead.getX() == BOARD_ROWS-1){
        snake.setDirection(LEFT);
    }
}