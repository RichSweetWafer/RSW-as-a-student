#include "widget.h"
#include "ui_widget.h"

Widget::Widget(Controls controls, size_t width, size_t height, IPresenter * presenter, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
    , presenter(presenter)
    , controls(controls)
{
    ui->setupUi(this);
    scene = new QGraphicsScene(this);
    ui->graphicsView->setScene(scene);

    rectangles.resize(height);
    for (int i = height - 1; i > -1; i--)
    {
        rectangles[i].resize(width);
        for (size_t k = 0; k < width; k++)
        {
            rectangles[i][k] = scene->addRect((0 + (40 * k)),
                                              (0 + ((height -(1 + i)) * 40)),
                                              40,
                                              40,
                                              QPen(Qt::NoPen),
                                              QBrush(Qt::red));
        }
    }
    text = scene->addText("Player stats");
    text->setPos(0, -20);
    this->resize((width + 1) * 40, (height + 1) * 40);
    this->setFocusPolicy(Qt::StrongFocus);
}
// callback
// Рассмотреть "callback"
void Widget::keyPressEvent(QKeyEvent *event)
{
    qDebug("Key pressed: %d", event->key());
    if (event->key() == controls.key_up)
    {
        presenter->move_up();
        qDebug("UP");
    }
    else if (event->key() == controls.key_down)
    {
        presenter->move_down();
        qDebug("DOWN");
    }
    else if (event->key() == controls.key_left)
    {
        presenter->move_left();
        qDebug("LEFT");
    }
    else if (event->key() == controls.key_right)
    {
        presenter->move_right();
        qDebug("RIGHT");
    }
    else if (event->key() == controls.key_attack)
    {
        presenter->player_attack();
        qDebug("ATTACK");
    }
}

void Widget::Print(Field &map, Player &player)
{
    //system("clear");
//    printf("\e[42;1;30mHP:%d  MP:%zu  STR:%zu\e[0m\n", player.get_hp(), player.get_mp(), player.get_strength());
    text->setPlainText("HP:" + QString::number(player.get_hp()) +
                       "  MP:" + QString::number(player.get_mp()) +
                       "  STR:" + QString::number(player.get_strength()));
    for (int i = map.get_height()-1; i > -1; i--)
    {
        for (size_t k = 0; k < map.get_width(); k++)
        {
            einhabitant inhab = map[i][k].get_inhabitant();
            if (inhab == PLAYER)
            {
                rectangles[i][k]->setBrush(Qt::green);
                //printf("\e[42m  \e[0m");
            }
            else if (inhab == ITEM)
            {
                rectangles[i][k]->setBrush(Qt::yellow);
//                printf("\e[43m  \e[0m");
            }
            else if (inhab == ENEMY)
            {
                rectangles[i][k]->setBrush(Qt::red);
//                printf("\e[40m  \e[0m");
            }
            else if (inhab == NOBODY)
            {
                estate state = map[i][k].get_state();
                if (state == WALL)
//                    printf("\e[41m  \e[0m");
                    rectangles[i][k]->setBrush(Qt::black);
                else if (state == PASS)
//                    printf("\e[47m  \e[0m");
                    rectangles[i][k]->setBrush(Qt::white);
                else if (state == START)
//                    printf("\e[44mSt\e[0m");
                    rectangles[i][k]->setBrush(Qt::darkMagenta);
                else if (state == EXIT)
//                    printf("\e[44mEn\e[0m");
                    rectangles[i][k]->setBrush(Qt::magenta);
            }
        }
//        std::cout << "\n";
    }
}

Widget::~Widget()
{
    delete scene;
    delete ui;
}

