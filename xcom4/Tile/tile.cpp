#include "tile.h"

Tile::Tile(estate state)
    :state(state),inhabitant(NOBODY){}

Tile::Tile(estate stat, einhabitant inhab)
    :state(stat), inhabitant(inhab){}

estate Tile::get_state()
{
    return state;
}

einhabitant Tile::get_inhabitant()
{
    return inhabitant;
}

void Tile::set_state(estate new_state)
{
    state = new_state;
}

void Tile::set_inhabitant(einhabitant new_inhabitant)
{
    if (state == PASS)
        inhabitant = new_inhabitant;
}

void Tile::set_object(IObject * obj)
{
    object = obj;
}

IObject * Tile::get_object()
{
    return object;
}


std::ostream& operator<<(std::ostream& stream, const Tile& tile)
{
    stream << "\n  State: " << std::to_string(tile.state)
       << "\n  Inhabitant: " << std::to_string(tile.inhabitant)
        << std::endl;
    return stream;
}


void Tile::write(QJsonObject &json) const
{
    json["estate"] = (int)state;
    json["einhabitant"] = (int)inhabitant;
}
