#pragma once
#include "Object.hpp"

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