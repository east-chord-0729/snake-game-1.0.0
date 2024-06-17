#include "Game.hpp"

int main()
{
    for (int i = 0; i < 4; i++)
    {
        Game game(i);

        //! display

        if (game.run() == false)
        {
            break;
        }
    }

    //! display

    return 0;
}