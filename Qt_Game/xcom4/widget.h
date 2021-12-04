#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QtCore>
#include <QtGui>
#include <QGraphicsScene>
#include <QKeyEvent>
#include <QGraphicsTextItem>
#include <QGraphicsRectItem>
#include "Controls/controls.h"
#include "Presenter/ipresenter.h"
#include "Field/field.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(Controls controls, size_t width, size_t height, IPresenter * presenter = nullptr, QWidget *parent = nullptr);
    void keyPressEvent(QKeyEvent * event);
    void Print(Field &map, Player &player);
    ~Widget();

private:
    Ui::Widget *ui;
    QGraphicsScene *scene;
    std::vector<std::vector<QGraphicsRectItem *>> rectangles;
    QGraphicsTextItem * text;
    IPresenter * presenter;
    Controls controls;
};
#endif // WIDGET_H
