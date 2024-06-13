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
    //! removeGate.

    /* event function */
    void moveSnake(Board &board, Snake &snake);
    void eatGoodItemAndMove(Board &board, Snake &snake);
    void eatBadItemAndMove(Board &board, Snake &snake);
    //! void readyToPassGate(Board &board, Snake &snake, Gate &gate1, Gate &gate2);
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
    addObjectToBoard(board, snake.getHead());
}

template <typename ItemType> void GameController::removeItem(Board &board, ItemType &item)
{
    board.addIcon(item.getY(), item.getX(), ICON_EMPTY);
}

void GameController::moveSnake(Board &board, Snake &snake)
{
    SnakeSegment nextHead = snake.getNextHead(); /* 다음 머리 */
    SnakeSegment removeTail = snake.getTail();   /* 사라질 꼬리 */

    /* 다음 머리 추가 */
    snake.addHead(nextHead);
    addObjectToBoard(board, nextHead);

    /* 꼬리 삭제 */
    snake.removeTail();
    addObjectToBoard(board, Empty(removeTail.getY(), removeTail.getX()));
}

void GameController::eatGoodItemAndMove(Board &board, Snake &snake)
{
    /* 다음 머리 추가 */
    SnakeSegment nextHead = snake.getNextHead();
    snake.addHead(nextHead);
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

// void GameController::readyToPassGate(Board &board, Snake &snake, Gate &gate1, Gate &gate2)
// {
//     SnakeSegment nextHead = snake.getNextHead();
//     SnakeSegment gatehead = SnakeSegment(0, 0); // 또다른 GATE가 들어갈 변수

//     if (nextHead.getY() == gate1.getY() && nextHead.getX() == gate1.getX())
//     {                                                        // GATE1에 닿았을 때
//         gatehead = SnakeSegment(gate2.getY(), gate2.getX()); // GATE2를 gatehead에 저장
//     }
//     else
//     {                                                        // GATE2에 닿았을 때
//         gatehead = SnakeSegment(gate1.getY(), gate1.getX()); // GATE1을 gatehead에 저장
//     }
//     snake.addHead(gatehead); // gatehead를 snake의 head로 설정
//     board.removeObject(snake.getTail());
//     snake.removeTail();

//     // GateDirection(gatehead); // GATE 방향 설정
// }