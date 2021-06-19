#include "imageviewer.h"

ImageViewer::ImageViewer()
{
    setMouseTracking(true);
}
ImageViewer::~ImageViewer()
{
    delete rubberBand;
}

void ImageViewer::mousePressEvent(QMouseEvent *mouseEvent)
{
    offset = mouseEvent->pos();
    if (flag == 1)
    {
        if (!rubberBand)
            rubberBand = new QRubberBand(QRubberBand::Rectangle, this);
        rubberBand->setGeometry(QRect(offset, QSize()));
        rubberBand->setWindowOpacity(0.0);
        rubberBand->setMinimumSize(4, 4);
        rubberBand->show();
        start = QPoint(offset.x(), this->height() - offset.y());
    }
}

void ImageViewer::mouseMoveEvent(QMouseEvent *mouseEvent)
{
    if (flag == 0)
    {
        if (mouseEvent->buttons() & Qt::LeftButton)
                this->move(mapToParent(mouseEvent->pos()) - offset);
    }
    else if (flag == 1)
    {
        if (rubberBand)
            rubberBand->setGeometry(QRect(offset, mouseEvent->pos()).normalized());
    }
}

void ImageViewer::mouseReleaseEvent(QMouseEvent * mouseEvent)
{
    if (flag == 1)
    {
        rubberBand->hide();
        end = QPoint(mouseEvent->pos().x(), this->height() - mouseEvent->pos().y());
        flag = 0;
        releasedAct.trigger();
    }
}
