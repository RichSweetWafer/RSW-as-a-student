#include "item.h"

Item::Item(eeffect eff, QPair<size_t, size_t> pos)
    :Object(OBJ_ITEM, 1, 30, pos), effect(eff){}

void Item::set_effect(eeffect eff)
{
    effect = eff;
}

eeffect Item::get_effect()
{
    return effect;
}

std::ostream& operator<<(std::ostream& stream, const Item& item)
{
    std::string type[] = {"HEALTH", "STRENGTH", "MANA"};
    QPair<size_t, size_t> pos = item.position;

    stream << "Item:\n  Position: " << std::to_string(pos.second) << ':' << std::to_string(pos.first)
       << "\n  Effect: " << type[item.effect]
       << "\n  MP: " << std::to_string(item.mp) << std::endl;
    return stream;
}
