#pragma once

#include "Object.hpp"
#include <ncurses.h>
#include <stdlib.h>
#include <time.h>

#define BOARD_COLS 13
#define BOARD_ROWS 25

class Board
{
  public:
    Board(int height = 0, int width = 0);
    void addObject(Object obj);
    void removeObject(Object obj);
    Object getObject(int x, int y);
    Object getObjectRand(chtype icon);
    void render();
    void clear();

  private:
    int width, height;
    WINDOW *boardWin;
};

Board::Board(int height, int width)
{
    int xMax, yMax, boardWidth, boardHeight;

    getmaxyx(stdscr, yMax, xMax);

    this->height = height;
    this->width = width;

    boardHeight = (yMax / 2) - (height / 2);
    boardWidth = (xMax / 2) - (width / 2);
    boardWin = newwin(height, width, boardHeight, boardWidth);

    wclear(boardWin);
    wrefresh(boardWin);
    box(boardWin, 0, 0);
}

void Board::addObject(Object obj)
{
    mvwaddch(boardWin, obj.getY(), obj.getX(), obj.getIcon());
}

void Board::removeObject(Object obj)
{
    mvwaddch(boardWin, obj.getY(), obj.getX(), ICON_EMPTY);
}

Object Board::getObject(int x, int y)
{
    chtype icon = mvwinch(boardWin, y, x);
    return Object(y, x, icon);
}

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