#include "snake.h"

Drawable::Drawable()
{
    y = x = 0;
    icon = ' ';
}

Drawable::Drawable(int y, int x, chtype ch)
{
    this->y = y;
    this->x = x;
    this->icon = ch;
}

int Drawable::getY()
{
    return y;
}

int Drawable::getX()
{
    return x;
}

chtype Drawable::getIcon()
{
    return icon;
}