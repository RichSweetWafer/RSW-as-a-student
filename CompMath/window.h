#ifndef WINDOW_H
#define WINDOW_H

#include "glwidget.h"
#include <QWidget>

QT_BEGIN_NAMESPACE
class QSlider;
class QPushButton;
QT_END_NAMESPACE

class GLWidget;
class MainWindow;

class Window : public QWidget
{
    Q_OBJECT

public:
    Window(MainWindow *mw);

private slots:
    void onStep();
    void onStart();
    void onPredict();
private:
    GLWidget* glWidget;
    QPushButton* startBtn;
    QPushButton* stepBtn;
    QPushButton* predictBtn;
    MainWindow* mainWindow;
};

#endif
