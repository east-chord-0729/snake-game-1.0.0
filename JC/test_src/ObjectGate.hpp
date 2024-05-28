#pragma once
#include "Object.hpp"

#include <stdlib.h>
#include <vector>
#include <algorithm>
class Gate : public Object
{
  public:
    Gate(int y = 0, int x = 0);
    void pairWith(Gate *other);
    Gate* getPairedGate();
    bool isPaired();
    Direction getExitDirection(Direction entryDirection);

  private:
    Gate *pairedGate;
    Direction determineExitDirection(Direction entryDirection);
};

Gate::Gate(int y, int x) : Object(y, x, ICON_GATE), pairedGate(nullptr) {}

void Gate::pairWith(Gate *other)
{
    pairedGate = other;
    other->pairedGate = this;
}

Gate* Gate::getPairedGate()
{
    return pairedGate;
}

bool Gate::isPaired()
{
    return pairedGate != nullptr;
}

Direction Gate::getExitDirection(Direction entryDirection)
{
    return determineExitDirection(entryDirection);
}

Direction Gate::determineExitDirection(Direction entryDirection)
{
    // If the gate is on the edge, return fixed direction
    if (y == 0) return DOWN; // Gate is on the top edge
    if (y == BOARD_ROWS - 1) return UP; // Gate is on the bottom edge
    if (x == 0) return RIGHT; // Gate is on the left edge
    if (x == BOARD_COLS - 1) return LEFT; // Gate is on the right edge

    // If the gate is in the middle, prioritize directions
    switch (entryDirection)
    {
        case UP:    return DOWN;
        case DOWN:  return UP;
        case LEFT:  return RIGHT;
        case RIGHT: return LEFT;
        default:    return UP; // Default direction
    }
}