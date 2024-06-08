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
    Gate gate1; //GATE 두개 추가
    Gate gate2;
    int gameSpeed;
    bool gameOver;
    int mapNum=3; //맵 선택
    int snakelength = 0;
    int gateLength = 0;

    void handleInput();
    void updateState();
    void render();
    void displayGameStart();
    void displayGameOver();
    void generateGate(); //GATE 생성
    void GateDirection(SnakeSegment gatehead); //GATE 방향 설정
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

    for(int i=0;i<BOARD_COLS;i++){ //맵 생성
        for(int j=0;j<BOARD_ROWS;j++){
            if(map[mapNum][i][j] == 1){ // 1은 Wall
                board.addObject(Wall(i,j)); 
            }
            else if(map[mapNum][i][j] == 2){ // 2는 ImmuneWall
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

    generateGate(); //GATE 생성
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
    do // Wall위에 gate1 생성
    {
        x = rand() % BOARD_COLS;
        y = rand() % BOARD_ROWS;
    } while (board.getObject(x, y).getIcon() != ICON_WALL);

    gate1 = Gate(y, x);
    board.addObject(gate1);

    do // Wall위에 gate2 생성
    {
        x = rand() % BOARD_COLS;
        y = rand() % BOARD_ROWS;
    } while (board.getObject(x, y).getIcon() != ICON_WALL);

    gate2 = Gate(y, x);
    board.addObject(gate2);
}

void SnakeGame::updateState()
{
    SnakeSegment nextHead = snake.getNextHead();
    Object collisionObject = board.getObject(nextHead.getX(), nextHead.getY());
    
    SnakeSegment gatehead = SnakeSegment(0, 0); // 또다른 GATE가 들어갈 변수

    if (snakelength != 0)
    {
        gateLength ++;
        if (gateLength == snakelength)
        {
            board.addObject(Wall(gate1.getY(), gate1.getX()));
            board.addObject(Wall(gate2.getY(), gate2.getX()));
            //generateGate();
            gateLength = 0;
            snakelength = 0;
        }
    }

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
        if (nextHead.getY() == gate1.getY() && nextHead.getX() == gate1.getX()) { // GATE1에 닿았을 때
            gatehead = SnakeSegment(gate2.getY(), gate2.getX()); // GATE2를 gatehead에 저장
        }
        else { // GATE2에 닿았을 때
            gatehead = SnakeSegment(gate1.getY(), gate1.getX()); // GATE1을 gatehead에 저장
        }
        snake.addHead(gatehead); // gatehead를 snake의 head로 설정
        board.removeObject(snake.getTail());
        snake.removeTail();

        GateDirection(gatehead); // GATE 방향 설정
        nextHead = snake.getNextHead(); // 방향 설정 후 다음 위치로 이동
        board.addObject(nextHead);
        snake.addHead(nextHead);
        board.removeObject(snake.getTail());
        snake.removeTail();
        snakelength = snake.getLength();
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
    //! 화면 싹 지우고 중심에 글 뜨게 하기
    //! 잠시후에 사라지지 말고, 버튼을 눌러야 사라지기.
    if(gameOver){
        mvprintw(0, 0, "Game Over! press any key to exit");
        mvprintw(1, 0, "Your snake length: %d", snake.getLength());
        refresh();
        napms(2000);
    }
}

void SnakeGame::GateDirection(SnakeSegment gatehead)
{
    SnakeSegment testnextHead = snake.getNextHead(); // 원래 방향으로 이동했을 때의 위치
    Object testcollisionObject = board.getObject(testnextHead.getX(), testnextHead.getY());
    
    if(gatehead.getY() == 0){ // GATE가 맵의 상단에 있을떄
        snake.setDirection(DOWN); // GATE의 방향을 DOWN으로 설정
    }
    else if(gatehead.getY() == BOARD_COLS-1){ // GATE가 맵의 하단에 있을때
        snake.setDirection(UP); // GATE의 방향을 UP으로 설정
    }
    else if(gatehead.getX() == 0){ // GATE가 맵의 좌측에 있을때
        snake.setDirection(RIGHT); // GATE의 방향을 RIGHT으로 설정
    }
    else if(gatehead.getX() == BOARD_ROWS-1){ // GATE가 맵의 우측에 있을때
        snake.setDirection(LEFT); // GATE의 방향을 LEFT으로 설정
    }
    else{ // GATE가 맵의 가장자리에 있지 않을때
        // 원래의 방향으로 갔을 떄 벽이 있을때만 방향을 바꿈
        if(testcollisionObject.getIcon() == ICON_WALL){ // 원래 방향으로 이동했을때 벽이 있을때
            switch (snake.getDirection())
            {
                case UP: // 원래 방향이 UP일때
                    snake.setDirection(RIGHT); // GATE의 방향을 RIGHT으로 설정
                    break;
                case DOWN: // 원래 방향이 DOWN일때
                    snake.setDirection(LEFT); // GATE의 방향을 LEFT으로 설정
                    break;
                case LEFT: // 원래 방향이 LEFT일때
                    snake.setDirection(UP); // GATE의 방향을 UP으로 설정
                    break;
                case RIGHT: // 원래 방향이 RIGHT일때
                    snake.setDirection(DOWN); // GATE의 방향을 DOWN으로 설정
                    break;
                default:
                    break;
            }
        }
    }
}