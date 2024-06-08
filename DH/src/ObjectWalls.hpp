#pragma once

#include "Object.hpp"

class Empty : public Object
{
  public:
    Empty(int y = 0, int x = 0)
    {
        this->y = y;
        this->x = x;
        this->icon = ICON_EMPTY;
    }
};

class Wall : public Object
{
  public:
    Wall(int y = 0, int x = 0)
    {
        this->y = y;
        this->x = x;
        this->icon = ICON_WALL;
    }
};

class ImmuneWall : public Object
{
  public:
    ImmuneWall(int y = 0, int x = 0)
    {
        this->y = y;
        this->x = x;
        this->icon = ICON_IWALL;
    }
};