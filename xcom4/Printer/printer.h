#ifndef PRINTER_H
#define PRINTER_H
#include "../Field/field.h"

class Printer
{
public:
    void Print(Field &map, Player &player);
    void Print(std::string str);
};

#endif // PRINTER_H
