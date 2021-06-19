#ifndef IMAGEVIEWER_H
#define IMAGEVIEWER_H

#include <QLabel>
#include <QMainWindow>
#include <QRubberBand>
#include <QAction>
#include <QMouseEvent>
#include <QPainter>
class ImageViewer : public QLabel
{
public:
    ImageViewer();
    ~ImageViewer();

    int flag = 0; //Events flag

    // Selection area points
    QPoint start;
    QPoint end;

    QAction releasedAct;

private slots:
    void mousePressEvent(QMouseEvent * mouseEvent) override;
    void mouseMoveEvent(QMouseEvent * mouseEvent) override;
    void mouseReleaseEvent(QMouseEvent * mouseEvent) override;

private:
    QPoint offset;
    QRubberBand * rubberBand = nullptr;
};

#endif // IMAGEVIEWER_H
