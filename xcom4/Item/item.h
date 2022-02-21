#ifndef ITEM_H
#define ITEM_H
#include "../Object/object.h"
#include <QJsonObject>

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
    void write(QJsonObject &json) const;

};

#endif // ITEM_H
