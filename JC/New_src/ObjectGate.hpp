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

  private:
    Gate *pairedGate;
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