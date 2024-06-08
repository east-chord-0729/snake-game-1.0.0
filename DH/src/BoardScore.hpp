#pragma once

#include <ncurses.h>

class BoardScore
{
  private:
    WINDOW *score_win;

  public:
    BoardScore(int y = 0, int x = 0);
    void render();
};

BoardScore::BoardScore(int y, int x)
{
}

void BoardScore::render()
{
    wrefresh(score_win);
}