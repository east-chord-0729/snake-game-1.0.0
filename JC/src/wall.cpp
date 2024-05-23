#include "snake.h"

Wall_G::Wall_G(int y, int x)
{
    this->y = y;
    this->x = x;
    this->icon = 'O';
}

Wall_NG::Wall_NG(int y, int x)
{
    this->y = y;
    this->x = x;
    this->icon = 'I';
}