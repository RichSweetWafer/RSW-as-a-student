#include "controls.h"

Controls::Controls(Qt::Key up, Qt::Key down,
                   Qt::Key left, Qt::Key right,
                   Qt::Key attack)
    :key_up(up),key_down(down),key_left(left),key_right(right),
    key_attack(attack){}
