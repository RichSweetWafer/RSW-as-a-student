#include "printer.h"

void Printer::Print(Field &map, Player& player)
{
    //system("clear");
    printf("\e[42;1;30mHP:%d  MP:%zu  STR:%zu\e[0m\n", player.get_hp(), player.get_mp(), player.get_strength());
    for (int i = map.get_height()-1; i > -1; i--)
    {
        for (size_t k = 0; k < map.get_width(); k++)
        {
            einhabitant inhab = map[i][k].get_inhabitant();
            if (inhab == PLAYER)
            {
                printf("\e[42m  \e[0m");
            }
            else if (inhab == ITEM)
            {
                printf("\e[43m  \e[0m");
            }
            else if (inhab == ENEMY)
            {
                printf("\e[40m  \e[0m");
            }
            else if (inhab == NOBODY)
            {
                estate state = map[i][k].get_state();
                if (state == WALL)
                    printf("\e[41m  \e[0m");
                else if (state == PASS)
                    printf("\e[47m  \e[0m");
                else if (state == START)
                    printf("\e[44mSt\e[0m");
                else if (state == EXIT)
                    printf("\e[44mEn\e[0m");
            }
        }
        std::cout << "\n";
    }
}

void Printer::Print(std::string str)
{
    std::cout << str << std::endl;
}
