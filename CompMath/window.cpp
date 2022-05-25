#include "window.h"

Window::Window(MainWindow *mw)
    : mainWindow(mw)
{
    glWidget = new GLWidget;

    QVBoxLayout* mainLayout = new QVBoxLayout;

    QHBoxLayout* buttonLayout = new QHBoxLayout;

    startBtn = new QPushButton(tr("Start"), this);
    connect(startBtn, &QPushButton::clicked, this, &Window::onStart);
    buttonLayout->addWidget(startBtn);

    stepBtn = new QPushButton(tr("Step"), this);
    connect(stepBtn, &QPushButton::clicked, this, &Window::onStep);
    buttonLayout->addWidget(stepBtn);

    predictBtn = new QPushButton(tr("Predict"), this);
    connect(predictBtn, &QPushButton::clicked, this, &Window::onPredict);
    buttonLayout->addWidget(predictBtn);

    QWidget *btns = new QWidget;
    btns->setLayout(buttonLayout);
    btns->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);


    QVBoxLayout* container = new QVBoxLayout;
    container->addWidget(btns);
    container->addWidget(glWidget);

    QWidget *w = new QWidget;
    w->setLayout(container);
    mainLayout->addWidget(w);

    setLayout(mainLayout);
}

void Window::onStart()
{
    glWidget->start();
}

void Window::onStep()
{

}

void Window::onPredict()
{
    bool ok;
    int rotateAngle = QInputDialog::getInt(this, tr("Object rotatrion angle"), tr("Value (from 0 to 359):"), 0, 0, 359, 1, &ok);
    if (ok)
    {
        int rayYAngle = QInputDialog::getInt(this, tr("Ray angle axis Y"), tr("Value (from 0 to 10):"), 0, 0, 10, 1, &ok);
        if (ok)
        {
            int rayZAngle = QInputDialog::getInt(this, tr("Ray angle axis Z"), tr("Value (from 0 to 10):"), 0, 0, 10, 1, &ok);
            if (ok)
            {
                QPair<double, double> pair = glWidget->predict(rotateAngle,
                                                               rayYAngle,
                                                               rayZAngle);
                QString str = "Predicted distance: ";
                str.append(QString::number(pair.second));
                str.append("\nActual distance: ");
                str.append(QString::number(pair.first));
                str.append("\n");
                QMessageBox::information(this, "Predict", str);
            }
        }
    }
}
