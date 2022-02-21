#ifndef CONTROLS_H
#define CONTROLS_H
#include <Qt>



class Controls
{
public:
    Qt::Key key_up;
    Qt::Key key_down;
    Qt::Key key_left;
    Qt::Key key_right;
    Qt::Key key_attack;
    Controls(Qt::Key up = Qt::Key::Key_W, Qt::Key down = Qt::Key::Key_S,
             Qt::Key left = Qt::Key::Key_A, Qt::Key right = Qt::Key::Key_D,
             Qt::Key attack = Qt::Key::Key_Space);
};

#endif // CONTROLS_H
