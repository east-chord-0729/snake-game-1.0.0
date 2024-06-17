#pragma once

#include "Object.hpp"

class Gate : public Object
{
  private:
    Direction direction;

  public:
    Gate(int y = 0, int x = 0)
    {
        this->y = y;
        this->x = x;
        this->icon = ICON_GATE;

        if (y == 0)
        {                     // GATE가 맵의 상단에 있을떄
            direction = DOWN; // GATE의 방향을 DOWN으로 설정
        }
        else if (y == BOARD_COLS - 1)
        {                   // GATE가 맵의 하단에 있을때
            direction = UP; // GATE의 방향을 UP으로 설정
        }
        else if (x == 0)
        {                      // GATE가 맵의 좌측에 있을때
            direction = RIGHT; // GATE의 방향을 RIGHT으로 설정
        }
        else if (x == BOARD_ROWS - 1)
        {                     // GATE가 맵의 우측에 있을때
            direction = LEFT; // GATE의 방향을 LEFT으로 설정
        }
        else
        {
            direction = NONE; // 방향 설정 안됨.
        }
    }

    Direction getDirection()
    {
        return this->direction;
    }

    void setDirection(Direction d)
    {
        direction = d;
    }
};