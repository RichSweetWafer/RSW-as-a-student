#ifndef REGULATOR_H
#define REGULATOR_H
#include "../Printer/printer.h"
#include "../widget.h"

template<size_t enemies = 3, size_t items = 3>
class Regulator
{
public:
    size_t enemy_count = enemies;
    size_t item_count = items;
    size_t return_enemies()
    {
        return enemy_count;
    };
    size_t return_items()
    {
        return item_count;
    };
};

#endif // REGULATOR_H
