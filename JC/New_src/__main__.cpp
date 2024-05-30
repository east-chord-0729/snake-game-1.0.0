#include "SnakeGame.hpp"

int main()
{
    SnakeGame game;
    game.run();
    mvprintw(2, 0, "Mission Complete!");
    refresh();
    napms(2000);
    return 0;
}