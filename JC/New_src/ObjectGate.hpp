#pragma once
#include "Object.hpp"

class Gate : public Object
{
    public:
        Gate(int y = 0, int x = 0)
        {
            this->y = y;
            this->x = x;
            this->icon = ICON_GATE;
        }
};