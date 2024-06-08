#pragma once

#include "Assets.hpp"
#include "Object.hpp" //! 삭제 예정
#include <ncurses.h>
#include <stdlib.h>
#include <time.h>

#define BOARD_COLS 13
#define BOARD_ROWS 25

class Board
{
  public:
    Board(int height = 0, int width = 0);

    void addIcon(int y, int x, chtype icon);
    chtype getIcon(int y, int x);
    void getRandomEmptyCoordinate(int *y, int *x);
    void getRandomWallCoordinate(int *y, int *x);

    //! 삭제 예정
    void addObject(Object obj);
    void removeObject(Object obj);
    Object getObject(int x, int y);
    Object getObjectRand(chtype icon);

    void render();
    void clear();

  private:
    int height, width;
    WINDOW *boardWin;
};

/**
 * @brief Construct a new Board::Board object
 *
 * @param height The vertical length of the board
 * @param width The horizontal length of the board
 */
Board::Board(int height, int width) : height(height), width(width)
{
    int yMax, xMax;

    getmaxyx(stdscr, yMax, xMax); /* Calculate the maximum width and height of the current screen */

    int pointY = (yMax / 2) - (height / 2); /* Starting y position to print the board */
    int pointX = (xMax / 2) - (width / 2);  /* Starting x position to print the board */

    boardWin = newwin(height, width, pointY, pointX); /* Create the window for the board */

    wclear(boardWin);    /* Clear the board window */
    wrefresh(boardWin);  /* Refresh the board window */
    box(boardWin, 0, 0); /* Border the board window */
}

void Board::addIcon(int y, int x, chtype icon)
{
    mvwaddch(boardWin, y, x, icon);
}

chtype Board::getIcon(int y, int x)
{
    return mvwinch(boardWin, y, x);
}

void Board::getRandomEmptyCoordinate(int *y, int *x)
{
    do
    {
        *y = rand() % height;
        *x = rand() % width;
    } while (getIcon(*y, *x) != ICON_EMPTY);
}

void Board::getRandomWallCoordinate(int *y, int *x)
{
    do
    {
        *y = rand() % height;
        *x = rand() % width;
    } while (getIcon(*y, *x) != ICON_WALL);
}

//! 삭제 예정
void Board::addObject(Object obj)
{
    mvwaddch(boardWin, obj.getY(), obj.getX(), obj.getIcon());
}

//! 삭제 예정
void Board::removeObject(Object obj)
{
    mvwaddch(boardWin, obj.getY(), obj.getX(), ICON_EMPTY);
}

//! 삭제 예정
Object Board::getObject(int x, int y)
{
    chtype icon = mvwinch(boardWin, y, x);
    return Object(y, x, icon);
}

//! 삭제 예정
Object Board::getObjectRand(chtype icon)
{
    int x, y;

    while (1)
    {
        y = rand() % height;
        x = rand() % width;
        icon = this->getObject(y, x).getIcon();

        if (icon == ICON_EMPTY)
        {
            break;
        }
    }

    return Object(y, x, icon);
}

void Board::render()
{
    wrefresh(boardWin);
}

void Board::clear()
{
    wclear(boardWin);
}