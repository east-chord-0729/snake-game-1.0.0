#pragma once

#include "Board.hpp"
#include "ObjectGate.hpp"
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
    Gate gate1;
    Gate gate2;
    int gameSpeed;
    bool gameOver;
    int mapNum=3;

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

    for(int i=0;i<BOARD_COLS;i++){
        for(int j=0;j<BOARD_ROWS;j++){
            if(map[mapNum][i][j] == 1){
                board.addObject(Wall(i,j));
            }
            else if(map[mapNum][i][j] == 2){
                board.addObject(ImmuneWall(i,j));
            }
        }
    }

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
    int x, y;
    do
    {
        x = rand() % BOARD_COLS;
        y = rand() % BOARD_ROWS;
    } while (board.getObject(x, y).getIcon() != ICON_WALL);

    gate1 = Gate(x, y);
    board.addObject(gate1);

    do
    {
        x = rand() % BOARD_COLS;
        y = rand() % BOARD_ROWS;
    } while (board.getObject(x, y).getIcon() != ICON_WALL);

    gate2 = Gate(x, y);
    board.addObject(gate2);
}

void SnakeGame::updateState()
{
    SnakeSegment nextHead = snake.getNextHead();
    Object collisionObject = board.getObject(nextHead.getX(), nextHead.getY());
    
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
        if (nextHead.getY() == gate1.getY() && nextHead.getX() == gate1.getX()) {
            gatehead = SnakeSegment(gate2.getY(), gate2.getX());
        }
        else {
            gatehead = SnakeSegment(gate1.getY(), gate1.getX());
        }
        snake.addHead(gatehead);
        board.removeObject(snake.getTail());
        snake.removeTail();
        GateDirection(gatehead);
        nextHead = snake.getNextHead();
        board.addObject(nextHead);
        snake.addHead(nextHead);
        board.removeObject(snake.getTail());
        snake.removeTail();
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
        mvprintw(1, 0, "Your snake length: %d", snake.getLength());
        refresh();
        napms(2000);
    }
}

void SnakeGame::GateDirection(SnakeSegment gatehead)
{
    SnakeSegment testnextHead = snake.getNextHead();
    Object testcollisionObject = board.getObject(testnextHead.getX(), testnextHead.getY());
    
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
    else{
        if(testcollisionObject.getIcon() == ICON_WALL){
            switch (snake.getDirection())
            {
                case UP:
                    snake.setDirection(RIGHT);
                    break;
                case DOWN:
                    snake.setDirection(LEFT);
                    break;
                case LEFT:
                    snake.setDirection(UP);
                    break;
                case RIGHT:
                    snake.setDirection(DOWN);
                    break;
                default:
                    break;
            }
        }
    }
}