#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "image.h"
#include "imageviewer.h"

#include <QApplication>

#include <QImage>
#include <QGraphicsView>

#include <QFileDialog>
#include <QInputDialog>
#include <QMessageBox>
#include <QtWidgets>

#include <QContextMenuEvent>
#include <QScrollBar>
#include <QKeyCombination>

   ///////////////////////////////////////////////////////////////
  /// The function definition lines in cpp file are indicated ///
 /// in the comment next to the function declaration.        ///
///////////////////////////////////////////////////////////////

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    // FILE MENU FUNCTIONS
    void Open(); // line 89
    void Save(); // line 116
    void SaveAs(); // line 130
    void about(); // line 156
    void exit(); // line 161

    // VIEW MENU FUNCTIONS
    void fitToWindow(); // line 168
    void zoomIn(); // line 176
    void zoomOut(); // line 181
    void normalSize(); // line 186
    void setImage(); // line 192

    // EDIT MENU FUNCTIONS
    void rgbChange(); // line 207
    void selection(); // line 224
    void rectangle(); // line 232
    void majorColorChange(); // line 327
    void split(); // line 338
    void backup(); // line 381

private:
    Image * image; // Image object pointer

    double scaleFactor = 1;

    QColor color;

    // ACTIONS AND MENUS MANAGING FUNCTIONS
    void createActions(); // line 391
    void updateActions(); // line 476
    void createMenus(); // line 489

    // IMAGE VIEW SCALING FUNCTIONS
    void scale(double factor); // line 516
    void adjustScrollBar(QScrollBar *scrollBar, double factor); // line 528

    // IMAGE VIEWER LABEL
    ImageViewer * imageViewer;
    QAction * releasedAct;

    // HELP LABEL
    QLabel * help;

    // SCROLLING
    QScrollArea * scrollArea;


    // MENUS
    QMenu * fileMenu;
    QMenu * viewMenu;
    QMenu * editMenu;

    // FILE MENU ACTIONS
    QAction * openAct;
    QAction * saveAct;
    QAction * saveAsAct;
    QAction * helpAct;
    QAction * exitAct;

    // VIEW MENU ACTIONS
    QAction * fitToWindowAct;
    QAction * normalSizeAct;
    QAction * zoomInAct;
    QAction * zoomOutAct;

    // EDIT MENU ACTIONS
    QAction * rgbChangeAct;
    QAction * selectionAct;
    QAction * majorColorChangeAct;
    QAction * splitAct;
    QAction * backupAct;

};
#endif // MAINWINDOW_H
