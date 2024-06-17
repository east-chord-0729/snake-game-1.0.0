#pragma once

class BoardMission
{
  public:
    BoardMission(int height = 0, int width = 0)
    {
        this->height = height;
        this->width = width;

        int yMax, xMax;
        getmaxyx(stdscr, yMax, xMax); /* Calculate the maximum width and height of the current screen */

        int pointY = (yMax / 2) - (height / 2) + 9;        /* Starting y position to print the board */
        int pointX = (xMax / 2) - (width / 2) + width + 1; /* Starting x position to print the board */

        win = newwin(9, 23, pointY, pointX); /* 창 생성 */
        box(win, 0, 0);                      /* 테두리 */
        init();                              /* 초기 미션 창 생성 */
    }

    void init()
    {
        mvwprintw(win, 1, 1, " ______MISSION______ "); /* 점수판 표기 */
        mvwprintw(win, 3, 2, "L: 10 [ ]");             /* 점수판 표기 */
        mvwprintw(win, 4, 2, "+: 5  [ ]");             /* 점수판 표기 */
        mvwprintw(win, 5, 2, "-: 2  [ ]");             /* 점수판 표기 */
        mvwprintw(win, 6, 2, "S: 1  [ ]");             /* 점수판 표기 */
        mvwprintw(win, 7, 2, "$: 1  [ ]");             /* 점수판 표기 */
    }

    void render()
    {
        wrefresh(win);
    }

    void updateMisson(int count, int code)
    {
        switch (code)
        {
        case 0:
            if (count == 10)
            {
                mvwprintw(win, 3 + code, 9, "v"); /* 점수판 표기 */
            }
            break;
        case 1:
            if (count == 5)
            {
                mvwprintw(win, 3 + code, 9, "v"); /* 점수판 표기 */
            }
            break;
        case 2:
            if (count == 2)
            {
                mvwprintw(win, 3 + code, 9, "v"); /* 점수판 표기 */
            }
            break;
        case 3:
            if (count == 1)
            {
                mvwprintw(win, 3 + code, 9, "v"); /* 점수판 표기 */
            }
            break;
        case 4:
            if (count == 1)
            {
                mvwprintw(win, 3 + code, 9, "v"); /* 점수판 표기 */
            }
            break;

        default:
            break;
        }
    }

  private:
    int height, width;
    WINDOW *win;
};