#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "window.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();
    void onReset();
};

#endif
