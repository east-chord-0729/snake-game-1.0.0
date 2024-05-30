#pragma once

#include <ncurses.h>

#define BOARD_COLS 13
#define BOARD_ROWS 25

#define ICON_EMPTY ' '
#define ICON_WALL 'O'
#define ICON_IWALL 'I'
#define ICON_SNAKE '#'
#define ICON_ITEM_GOOD 'G'
#define ICON_ITEM_BAD 'B'
#define ICON_ITEM_SLOW 'S'
#define ICON_GATE '$'

enum Direction
{
    UP,
    DOWN,
    LEFT,
    RIGHT
};

class Object
{
  public:
    Object(int y = 0, int x = 0, chtype icon = ICON_EMPTY);
    int getY();
    int getX();
    chtype getIcon();
    void setCoord(int y, int x);

  protected:
    int y, x;
    chtype icon;
};

Object::Object(int y, int x, chtype icon)
{
    this->y = y;
    this->x = x;
    this->icon = icon;
}

int Object::getY()
{
    return y;
}

int Object::getX()
{
    return x;
}

chtype Object::getIcon()
{
    return icon;
}

void Object::setCoord(int y, int x)
{
    this->y = y;
    this->x = x;
}