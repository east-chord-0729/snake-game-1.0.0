#pragma once

#include "Object.hpp"

class GoodItem : public Object
{
  public:
    GoodItem(int y = 0, int x = 0)
    {
        this->y = y;
        this->x = x;
        this->icon = ICON_ITEM_GOOD;
    }
};

class BadItem : public Object
{
  public:
    BadItem(int y = 0, int x = 0)
    {
        this->y = y;
        this->x = x;
        this->icon = ICON_ITEM_BAD;
    }
};

class SlowItem : public Object
{
  public:
    SlowItem(int y = 0, int x = 0)
    {
        this->y = y;
        this->x = x;
        this->icon = ICON_ITEM_SLOW;
    }
};