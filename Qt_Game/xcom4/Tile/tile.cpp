#include "tile.h"

Tile::Tile(estate state)
    :state(state),inhabitant(NOBODY){}

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
