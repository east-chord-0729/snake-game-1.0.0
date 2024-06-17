#pragma once

#include "Board.hpp"
#include "ncurses.h"

class BoardScore
{
  public:
    BoardScore(int height = 0, int width = 0)
    {
        this->height = height;
        this->width = width;

        int yMax, xMax;
        getmaxyx(stdscr, yMax, xMax); /* Calculate the maximum width and height of the current screen */

        int pointY = (yMax / 2) - (height / 2);            /* Starting y position to print the board */
        int pointX = (xMax / 2) - (width / 2) + width + 1; /* Starting x position to print the board */

        win = newwin(9, 23, pointY, pointX); /* 창 생성 */
        box(win, 0, 0);                      /* 테두리 */
        init();
    }

    void init()
    {
        mvwprintw(win, 1, 1, " ____SCORE BOARD____ "); /* 점수판 표기 */
        mvwprintw(win, 3, 2, "L: 0 / 20");             /* 점수판 표기 */
        mvwprintw(win, 4, 2, "+: 0");                  /* 점수판 표기 */
        mvwprintw(win, 5, 2, "-: 0");                  /* 점수판 표기 */
        mvwprintw(win, 6, 2, "S: 0");                  /* 점수판 표기 */
        mvwprintw(win, 7, 2, "$: 0");                  /* 점수판 표기 */
    }

    void render()
    {
        wrefresh(win);
    }

    void updateScore(int score, int code)
    {
        mvwprintw(win, 3 + code, 5, "%llu", score);
    }

  private:
    int height, width;
    WINDOW *win;
};