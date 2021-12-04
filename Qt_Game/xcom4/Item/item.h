#ifndef ITEM_H
#define ITEM_H
#include "../Object/object.h"

enum eeffect {HEALTH, STRENGTH, MANA};

class Item : public Object
{
private:
    eeffect effect;
public:
    Item(eeffect eff, QPair<size_t, size_t> pos);
    void set_effect(eeffect eff);
    eeffect get_effect();

    friend std::ostream& operator<<(std::ostream& stream, const Item& item);
};

#endif // ITEM_H
