#ifndef IPRESENTER_H
#define IPRESENTER_H

class IPresenter
{
public:
    virtual void move_up() = 0;
    virtual void move_down() = 0;
    virtual void move_left() = 0;
    virtual void move_right() = 0;
    virtual void player_attack() = 0;
    virtual void update() = 0;
    virtual int exec() = 0;
};
#endif // IPRESENTER_H
