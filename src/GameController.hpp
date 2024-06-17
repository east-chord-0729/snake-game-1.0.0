#pragma once

#include "Board.hpp"
#include "ObjectGate.hpp"
#include "ObjectItems.hpp"
#include "ObjectSnake.hpp"
#include "ObjectWalls.hpp"

class GameController
{
  public:
    void addObjectToBoard(Board &board, Object object);
    Object getObjectInBoard(Board &board, int y, int x);

    /* generate functions */
    void generateWalls(Board &board, int level);
    void generateGates(Board &board, Gate &gate1, Gate &gate2);
    template <typename ItemType> void generateItem(Board &board, ItemType &item);
    void generateSnake(Board &board, Snake &snake);

    /* remove function */
    template <typename ItemType> void removeItem(Board &board, ItemType &item);
    void removeGate(Board &board, Gate &gate1, Gate &gate2);

    /* event function */
    void moveSnake(Board &board, Snake &snake);
    void eatGoodItemAndMove(Board &board, Snake &snake);
    void eatBadItemAndMove(Board &board, Snake &snake);
    void passGate(Board &board, Snake &snake, Gate &gate1, Gate &gate2);
    void GateDirection(Board &board, Snake &snake, SnakeSegment &gateHead);
    void checkGates(Board &board, Gate &gate1, Gate &gate2, int &gateLength, int &snakeLength);
};

void GameController::addObjectToBoard(Board &board, Object object)
{
    board.addIcon(object.getY(), object.getX(), object.getIcon());
}

void GameController::generateWalls(Board &board, int level)
{
    for (int i = 0; i < BOARD_COLS; i++)
    {
        for (int j = 0; j < BOARD_ROWS; j++)
        {
            int objectCode = map[level][i][j];

            switch (objectCode)
            {
            case 1:
                addObjectToBoard(board, Wall(i, j));
                break;
            case 2:
                addObjectToBoard(board, ImmuneWall(i, j));
                break;
            default:
                addObjectToBoard(board, Empty(i, j));
                break;
            }
        }
    }
}

void GameController::generateGates(Board &board, Gate &gate1, Gate &gate2)
{
    int y, x;
    board.getRandomWallCoordinate(&y, &x);
    gate1 = Gate(y, x);
    board.addIcon(y, x, gate1.getIcon());
    board.getRandomWallCoordinate(&y, &x);
    gate2 = Gate(y, x);
    board.addIcon(y, x, gate2.getIcon());
}

template <typename ItemType> void GameController::generateItem(Board &board, ItemType &item)
{
    int y, x;
    board.getRandomEmptyCoordinate(&y, &x);
    item = ItemType(y, x);
    board.addIcon(y, x, item.getIcon());
}

void GameController::generateSnake(Board &board, Snake &snake)
{
    snake = Snake(3, 5, RIGHT); // TODO 랜덤으로 생성할 수 있나?
    addObjectToBoard(board, snake.getHead());

    snake.addHead(snake.getNextHead());
    addObjectToBoard(board, snake.getHead());

    snake.addHead(snake.getNextHead());
    SnakeSegment head = snake.getHead();
    head.setIcon(ICON_SNAKE_HEAD);
    addObjectToBoard(board, head);
}

template <typename ItemType> void GameController::removeItem(Board &board, ItemType &item)
{
    board.addIcon(item.getY(), item.getX(), ICON_EMPTY);
}

// gate1, gate2 삭제 함수 추가
void GameController::removeGate(Board &board, Gate &gate1, Gate &gate2)
{
    board.addIcon(gate1.getY(), gate1.getX(), ICON_WALL);
    board.addIcon(gate2.getY(), gate2.getX(), ICON_WALL);
}

void GameController::moveSnake(Board &board, Snake &snake)
{
    SnakeSegment nextHead = snake.getNextHead(); /* 다음 머리 */
    SnakeSegment privHead = snake.getHead();     /* 현재 머리 */
    SnakeSegment removeTail = snake.getTail();   /* 사라질 꼬리 */

    /* 다음 머리 추가 */
    snake.addHead(nextHead);
    nextHead.setIcon(ICON_SNAKE_HEAD);
    privHead.setIcon(ICON_SNAKE);
    addObjectToBoard(board, privHead);
    addObjectToBoard(board, nextHead);

    /* 꼬리 삭제 */
    snake.removeTail();
    addObjectToBoard(board, Empty(removeTail.getY(), removeTail.getX()));
}

void GameController::eatGoodItemAndMove(Board &board, Snake &snake)
{
    SnakeSegment nextHead = snake.getNextHead();
    SnakeSegment privHead = snake.getHead();

    snake.addHead(nextHead);
    nextHead.setIcon(ICON_SNAKE_HEAD);
    privHead.setIcon(ICON_SNAKE);
    addObjectToBoard(board, privHead);
    addObjectToBoard(board, nextHead);
}

void GameController::eatBadItemAndMove(Board &board, Snake &snake)
{
    /* 꼬리 삭제 */
    SnakeSegment tail = snake.getTail();
    snake.removeTail();
    addObjectToBoard(board, Empty(tail.getY(), tail.getX()));

    /* 이동 */
    moveSnake(board, snake);
}

void GameController::passGate(Board &board, Snake &snake, Gate &gate1, Gate &gate2)
{
    SnakeSegment nextHead = snake.getNextHead();
    SnakeSegment gateHead = SnakeSegment(0, 0); // 또다른 GATE가 들어갈 변수
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

    GateDirection(board, snake, gateHead); // GATE 방향 설정
}

void GameController::GateDirection(Board &board, Snake &snake, SnakeSegment &gateHead)
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

void GameController::checkGates(Board &board, Gate &gate1, Gate &gate2, int &gateLength, int &snakeLength)
{
    if (snakeLength != 0)
    {
        gateLength++;
        if (gateLength == snakeLength)
        {
            board.addIcon(gate1.getY(), gate1.getX(), ICON_WALL);
            board.addIcon(gate2.getY(), gate2.getX(), ICON_WALL);
            generateGates(board, gate1, gate2);
            gateLength = 0;
            snakeLength = 0;
        }
    }
}