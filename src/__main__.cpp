#include "Game.hpp"

int main()
{
    for (int i = 0; i < 4; i++)
    {
        Game game(i);

        if (game.run() == false)
        {
            break;
        }
    }

    return 0;
}