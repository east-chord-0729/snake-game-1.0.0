#pragma once

#include <ncurses.h>

#define BOARD_COLS 25
#define BOARD_ROWS 30

#define ICON_EMPTY ' '
#define ICON_WALL 'O'
#define ICON_IWALL 'I'
#define ICON_SNAKE '#'
#define ICON_SNAKE_HEAD '@'
#define ICON_ITEM_GOOD 'G'
#define ICON_ITEM_BAD 'B'
#define ICON_ITEM_SLOW 'S'
#define ICON_GATE '$'

enum Direction
{
    UP,
    DOWN,
    LEFT,
    RIGHT,
    NONE
};

class Object
{
  protected:
    int y, x;
    chtype icon;

  public:
    Object(int y = 0, int x = 0, chtype icon = ICON_EMPTY);

    /* get */
    int getY();
    int getX();
    chtype getIcon();

    /* set */
    void setY(int y);
    void setX(int x);
    void setIcon(chtype icon);
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

void Object::setY(int y)
{
    this->y = y;
}

void Object::setX(int x)
{
    this->x = x;
}

void Object::setIcon(chtype icon)
{
    this->icon = icon;
}