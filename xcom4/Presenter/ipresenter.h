#ifndef IPRESENTER_H
#define IPRESENTER_H
#include <QString>
#include <QJsonObject>
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
    virtual bool save() = 0;
    virtual void write(QJsonObject &json) = 0;
};
#endif // IPRESENTER_H
