#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , image(new Image)
    , imageViewer(new ImageViewer())
    , help(new QLabel())
    , scrollArea(new QScrollArea)
{
    imageViewer->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    imageViewer->setScaledContents(true);
    imageViewer->setAlignment(Qt::AlignCenter);
    releasedAct = &(imageViewer->releasedAct);
    connect(releasedAct, &QAction::triggered, this, &MainWindow::rectangle);

    scrollArea->setBackgroundRole(QPalette::Shadow);
    scrollArea->setWidget(imageViewer);
    scrollArea->setVisible(false);
    setCentralWidget(scrollArea);

    help->setText(tr("ABOUT\n\nCute Editor 1.0.0\nBased on QT 5.15.2\n"
                     "Author: Zachary Samoilov, Saint Petersburg ETU student\n"
                     "The program is provided AS IS with NO WARRANTY OF ANY KIND.\n"
                     "\nHELP\n\nThe program performs actions ONLY on BitMap images with depth of 24."
                     "\nUser may use menu bar or shortcuts to perform actions."
                     "\nAfter opening an image, User can move it with mouse inside the window."
                     "\n\nWhile all actions in the \"File\" menu are self-explanatory, others must be clarified."
                     "\n\"View\" menu provides tools for resizing the representation of the opened image."
                     "\n\"Fit to window\" sets image size to fit the window borders, blocking moving and zoomin while active."
                     "\n\nNote: If the image was somehow placed outside of the window's borders,"
                     "\nUser must press \"Normal Size\" button in the \"View\" menu to return it to it's original place and size."
                     "\n\n\"Edit\" menu contains options for changing the image itself:"
                     "\n\t\"Change RGB Component\" sets Red, Green or Blue component"
                     "\n\tto the given value in range from 0 to 255."
                     "\n\n\t\"Selection for shuffle/draw/fill\" allows User to select a rectangular area via mouse and action to apply to this area."
                     "\n\tShuffling splits the area into 4 parts and shuffles them either clockwise or diagonally."
                     "\n\tDrawing option can be used to fill the area with an optional color or to create a frame of the selected thickness."
                     "\n\tThickness of 0 will fill the area."
                     "\n\tFilling stands for copying the area and filling the image with it."
                     "\n\n\t\"Split\" can be performed to make a grid on the image or to cut image into N times M small images (leftovers won't be saved)."
                     "\n\n\n"));
    help->setWindowTitle(tr("About"));
    help->setFixedSize(help->sizeHint());

    createActions();
    createMenus();

    setWindowTitle("Cute Editor");
    resize(QGuiApplication::primaryScreen()->availableSize() * 4 / 5);
}

MainWindow::~MainWindow()
{
    delete image;
    delete imageViewer;

    delete scrollArea;

    delete fileMenu;
    delete viewMenu;
    delete editMenu;

    delete openAct;
    delete saveAct;
    delete saveAsAct;
    delete helpAct;
    delete exitAct;

    delete fitToWindowAct;
    delete normalSizeAct;
    delete zoomInAct;
    delete zoomOutAct;

    delete rgbChangeAct;
    delete selectionAct;
    delete majorColorChangeAct;
    delete splitAct;
    delete backupAct;

    delete help;

    QFile::remove("tmp.bmp");
    QFile::remove("backup.bmp");
}

// FILE MENU FUNCTIONS

void MainWindow::Open()
{
    QString filepath = QFileDialog::getOpenFileName(
                this,
                tr("Open a file"),
                QDir::homePath(),
                "Images (*.bmp)");
    if (!filepath.isEmpty())
    {
        image->Read(filepath);
        if (!image->lastRead)
        {
            QMessageBox::information(this, tr("Load error"), "Could not load the file", QMessageBox::Warning);
        }
        else
        {
            setImage();

            image->updatePixmap();
            QString message = filepath +  " has been opened";
            QMessageBox::information(this, tr("File Name"), message);
        }
    }
    else
        QMessageBox::information(this, tr("Open"), "Nothing to open", QMessageBox::Information);
}

void MainWindow::Save()
{
    if (image == nullptr)
    {
        QMessageBox::information(this, tr("Error"), "You don't have anything to save yet", QMessageBox::Information);

    }
    else
    {
        image->Export();
        QMessageBox::information(this, tr("Save"), "Success");
    }
}

void MainWindow::SaveAs()
{
    if (image == nullptr)
    {
        QMessageBox::information(this, tr("Error"), "You don't have anything to save yet", QMessageBox::Information);

    }
    else
    {
        QString filepath = QFileDialog::getSaveFileName(
                    this,
                    tr("Save File"),
                    QDir::homePath(),
                    "Images (*.bmp)");
        if (!filepath.isEmpty())
        {
            filepath.append(".bmp");
            image->Export(filepath);
            QMessageBox::information(this, tr("Save"), "Success");

        }
        else
            QMessageBox::information(this, tr("Save"), "Please, give the file a name", QMessageBox::Information);
    }
}

void MainWindow::about()
{
    help->show();
}

void MainWindow::exit()
{
    QCoreApplication::exit();
}

// VIEW MENU FUNCTIONS

void MainWindow::fitToWindow()
{
    scrollArea->setWidgetResizable(fitToWindowAct->isChecked());
    if (!fitToWindowAct->isChecked())
        normalSize();
    updateActions();
}

void MainWindow::zoomIn()
{
    scale(1.25);
}

void MainWindow::zoomOut()
{
    scale(0.75);
}

void MainWindow::normalSize()
{
    imageViewer->adjustSize();
    scaleFactor = 1.0;
}

void MainWindow::setImage()
{
    imageViewer->setPixmap(*image->pixmap);
    scaleFactor = 1.0;

    scrollArea->setVisible(true);
    fitToWindowAct->setEnabled(true);
    updateActions();
    image->updatePixmap();
    if (!fitToWindowAct->isChecked())
        imageViewer->adjustSize();
}

// EDIT MENU FUNCTIONS

void MainWindow::rgbChange()
{
    QStringList colours;
    colours << tr("Red") << tr("Green") << tr("Blue");
    bool ok;
    QString colour = QInputDialog::getItem(this, tr("Change colour"), tr("Colour: "), colours, 0, false, &ok);

    if (ok && !colour.isEmpty())
    {
        int num = QInputDialog::getInt(this, tr("Change colour"), tr("Value (from 0 to 255):"), 0, 0, 255, 1, &ok);
        if (ok)
            image->setRGB(colour, num);
    }
    imageViewer->setPixmap(*image->pixmap);
    updateActions();
}

void MainWindow::selection()
{
    if (imageViewer->flag == 1)
        imageViewer->flag = 0;
    else
        imageViewer->flag = 1;
}

void MainWindow::rectangle()
{
    QPoint selectionOrigin = imageViewer->start;
    QPoint selectionEnd = imageViewer->end;

    int x_origin = selectionOrigin.x() * image->m_width / imageViewer->width();
    int y_origin = selectionOrigin.y() * image->m_height / imageViewer->height();

    int x_end = selectionEnd.x() * image->m_width / imageViewer->width();
    int y_end = selectionEnd.y() * image->m_height / imageViewer->height();
    QStringList actions;
    actions << tr("Draw rectangle") << tr("Shuffle") << tr("Fill image") << tr("Fill outside rectangle");

    bool ok = true;
    if (x_end >= image->m_width || x_end < 0 || y_end < 0 || y_end >= image->m_height)
    {
        QMessageBox::information(this, tr("Error"), "Please, keep selection area inside the image", QMessageBox::Information);
        ok = false;
    }
    QString action;
    if (ok)
        action = QInputDialog::getItem(this, tr("Shuffle, Draw, Fill image or Fill outside"), tr("Action: "), actions, 0, false, &ok);

    if (ok)
    {
        if (x_origin > x_end && y_origin < y_end)
        {
            int tmp = x_origin;
            x_origin = x_end;
            x_end = tmp;
            tmp = y_origin;
            y_origin = y_end;
            y_end = tmp;
        }
        else if (x_origin < x_end && y_origin < y_end)
        {
            int tmp = y_origin;
            y_origin = y_end;
            y_end = tmp;
        }
        else if (x_origin > x_end && y_origin > y_end)
        {
            int tmp = x_origin;
            x_origin = x_end;
            x_end = tmp;
        }

        if (action == "Draw rectangle")
        {
            int pts = QInputDialog::getInt(this, tr("Line thickness"), tr("Value of 0 will fill the area"), 0, 0, 15, 1, &ok);
            if (ok)
            {
                color = QColorDialog::getColor(tr("Choose Color"));
                int r, g, b;
                color.getRgb(&r, &g, &b);
                if (pts != 0)
                    image->rectangleDraw(QPoint(x_origin, y_origin), QPoint(x_end, y_end), Color(static_cast<float>(r) / 255.f, static_cast<float>(g) / 255.f, static_cast<float>(b) / 255.f), pts);
                else
                    image->rectangleFill(QPoint(x_origin, y_origin), QPoint(x_end, y_end), Color(static_cast<float>(r) / 255.f, static_cast<float>(g) / 255.f, static_cast<float>(b) / 255.f));
            }

        }
        else if (action == "Shuffle")
        {
            QStringList options;
            options << tr("Diagonal") << tr("Clockwise");
            QString option = QInputDialog::getItem(this, tr("Choose method"), tr("Method: "), options, 0, false, &ok);
            if (ok)
            {
                if ((y_origin - y_end) % 2 != 0)
                    y_end++;
                if((x_end - x_origin) % 2 != 0)
                    x_end--;
                if (option == "Diagonal")
                    image->rectangleShuffle(QPoint(x_origin, y_origin), QPoint(x_end, y_end), 0);
                else if (option == "Clockwise")
                    image->rectangleShuffle(QPoint(x_origin, y_origin), QPoint(x_end, y_end), 1);
            }
        }
        else if (action == "Fill image")
        {
            image->fill(QPoint(x_origin, y_origin), QPoint(x_end, y_end));
        }
        else if (action == "Fill outside rectangle")
        {
            color = QColorDialog::getColor(tr("Choose Color"));
            int r, g, b;
            color.getRgb(&r, &g, &b);
            image->rectangleFillOutside(QPoint(x_origin, y_origin), QPoint(x_end, y_end), Color(static_cast<float>(r) / 255.f, static_cast<float>(g) / 255.f, static_cast<float>(b) / 255.f));
        }
    }
    imageViewer->setPixmap(*image->pixmap);
    updateActions();
}

void MainWindow::majorColorChange()
{
    color = QColorDialog::getColor(tr("Choose Color"));
    int r, g, b;
    color.getRgb(&r, &g, &b);

    image->majorColorChange(Color(static_cast<float>(r) / 255.f, static_cast<float>(g) / 255.f, static_cast<float>(b) / 255.f));
    imageViewer->setPixmap(*image->pixmap);
    updateActions();
}

void MainWindow::split()
{
    int v, h;
    bool ok = false;
    int pts = QInputDialog::getInt(this, tr("Thickness"), tr("value of zero will cut image into small ones"), 0, 0, 15, 1, &ok);
    if (ok && pts != 0)
    {
        v = QInputDialog::getInt(this, tr("Vertical split"), tr("pieces"), 1, 1, 100, 1, &ok);
        if (ok)
        {
            h = QInputDialog::getInt(this, tr("Horisontal split"), tr("pieces"), 1, 1, 100, 1, &ok);
            if (ok)
            {
                color = QColorDialog::getColor(tr("Choose Color"));
                int r, g, b;
                color.getRgb(&r, &g, &b);
                image->split(v, h, pts, Color(static_cast<float>(r) / 255.f, static_cast<float>(g) / 255.f, static_cast<float>(b) / 255.f));
            }
        }
    }
    else if (ok && pts == 0)
    {
        QString filepath = QFileDialog::getExistingDirectory(
                    this,
                    tr("Save Files"),
                    QDir::homePath());
        if (!filepath.isEmpty())
        {
            h = QInputDialog::getInt(this, tr("Horisontal split"), tr("pieces"), 1, 1, 8, 1, &ok);
            if (ok)
            {
                v = QInputDialog::getInt(this, tr("Vertical split"), tr("pieces"), 1, 1, 8, 1, &ok);
                if (ok)
                {
                    image->split(h, v, filepath);
                }
            }
        }
    }
    imageViewer->setPixmap(*image->pixmap);
    updateActions();
}

void MainWindow::backup()
{
    QString backup = "backup.bmp";
    image->Read(backup);
    setImage();
    updateActions();
}

// ACTIONS AND MENUS MANAGING

void MainWindow::createActions()
{
    // FILE MENU ACTIONS

    openAct = new QAction(tr("Open"), this);
    openAct->setShortcuts(QKeySequence::Open);
    openAct->setStatusTip(tr("Open file"));
    connect(openAct, &QAction::triggered, this, &MainWindow::Open);

    saveAct = new QAction(tr("Save"), this);
    saveAct->setShortcuts(QKeySequence::Save);
    saveAct->setStatusTip(tr("Save file"));
    connect(saveAct, &QAction::triggered, this, &MainWindow::Save);

    saveAsAct = new QAction(tr("Save as"), this);
    saveAsAct->setStatusTip(tr("Save file"));
    saveAsAct->setShortcuts(QKeySequence::SaveAs);
    connect(saveAsAct, &QAction::triggered, this, &MainWindow::SaveAs);

    helpAct = new QAction(tr("About/Help..."), this);
    helpAct->setEnabled(true);
    helpAct->setShortcut(tr("Ctrl+H"));
    helpAct->setStatusTip(tr("About/Help"));
    connect(helpAct, &QAction::triggered, this, &MainWindow::about);

    exitAct = new QAction(tr("Exit"), this);
    exitAct->setShortcut(QKeySequence::Quit);
    exitAct->setStatusTip(tr("Close program without saving"));
    connect(exitAct, &QAction::triggered, this, &MainWindow::exit);

    // VIEW MENU ACTIONS

    fitToWindowAct = new QAction(tr("Fit to window"), this);
    fitToWindowAct->setEnabled(false);
    fitToWindowAct->setCheckable(true);
    fitToWindowAct->setShortcut(tr("Ctrl+F"));
    fitToWindowAct->setStatusTip(tr("Set image size "));
    connect(fitToWindowAct, &QAction::triggered, this, &MainWindow::fitToWindow);

    zoomInAct = new QAction(tr("Zoom In"), this);
    zoomInAct->setEnabled(false);
    zoomInAct->setShortcut(QKeySequence::ZoomIn);
    connect(zoomInAct, &QAction::triggered, this, &MainWindow::zoomIn);

    zoomOutAct = new QAction(tr("Zoom Out"), this);
    zoomOutAct->setEnabled(false);
    zoomOutAct->setShortcut(QKeySequence::ZoomOut);
    connect(zoomOutAct, &QAction::triggered, this, &MainWindow::zoomOut);

    normalSizeAct = new QAction(tr("Normal Size"), this);
    normalSizeAct->setEnabled(false);
    normalSizeAct->setStatusTip(tr("Set scale factor to 1"));
    connect(normalSizeAct, &QAction::triggered, this, &MainWindow::normalSize);

    // EDIT MENU ACTIONS

    rgbChangeAct = new QAction(tr("Change RGB component..."), this);
    rgbChangeAct->setShortcut(tr("Ctrl+E"));
    rgbChangeAct->setEnabled(false);
    connect(rgbChangeAct, &QAction::triggered, this, &MainWindow::rgbChange);

    majorColorChangeAct = new QAction(tr("Change the most common colour..."), this);
    majorColorChangeAct->setShortcut(tr("Ctrl+T"));
    majorColorChangeAct->setEnabled(false);
    connect(majorColorChangeAct, &QAction::triggered, this, &MainWindow::majorColorChange);

    selectionAct = new QAction(tr("Selection for shuffle/draw/fill"), this);
    selectionAct->setEnabled(false);
    selectionAct->setShortcut(tr("Ctrl+R"));
    selectionAct->setStatusTip(tr("Draw a rectangle"));
    connect(selectionAct, &QAction::triggered, this, &MainWindow::selection);

    splitAct = new QAction(tr("Split image"), this);
    splitAct->setEnabled(false);
    splitAct->setShortcut(tr("Ctrl+Y"));
    splitAct->setStatusTip(tr("Split"));
    connect(splitAct, &QAction::triggered, this, &MainWindow::split);

    backupAct = new QAction(tr("Undo"), this);
    backupAct->setEnabled(false);
    backupAct->setShortcut(tr("Ctrl+Z"));
    backupAct->setStatusTip(tr("Undo the last action"));
    connect(backupAct, &QAction::triggered, this, &MainWindow::backup);
}

void MainWindow::updateActions()
{
    normalSizeAct->setEnabled(!fitToWindowAct->isChecked());
    zoomInAct->setEnabled(!fitToWindowAct->isChecked());
    zoomOutAct->setEnabled(!fitToWindowAct->isChecked());
    rgbChangeAct->setEnabled(!image->isEmpty());
    majorColorChangeAct->setEnabled(!image->isEmpty());
    selectionAct->setEnabled(!image->isEmpty());
    splitAct->setEnabled(!image->isEmpty());
    backupAct->setEnabled(QFile::exists("backup.bmp"));

}

void MainWindow::createMenus()
{
    // FILE MENU
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(openAct);
    fileMenu->addAction(saveAct);
    fileMenu->addAction(saveAsAct);
    fileMenu->addSeparator();
    fileMenu->addAction(helpAct);
    fileMenu->addAction(exitAct);
    // VIEW MENU
    viewMenu = menuBar()->addMenu(tr("&View"));
    viewMenu->addAction(fitToWindowAct);
    viewMenu->addAction(zoomInAct);
    viewMenu->addAction(zoomOutAct);
    viewMenu->addAction(normalSizeAct);
    // EDIT MENU
    editMenu = menuBar()->addMenu(tr("&Edit"));
    editMenu->addAction(rgbChangeAct);
    editMenu->addAction(selectionAct);
    editMenu->addAction(majorColorChangeAct);
    editMenu->addAction(splitAct);
    editMenu->addAction(backupAct);
}

// IMAGE VIEW SCALING FUNCTIONS

void MainWindow::scale(double factor)
{
    scaleFactor *= factor;
    imageViewer->resize(scaleFactor * imageViewer->pixmap(Qt::ReturnByValue).size());

    adjustScrollBar(scrollArea->horizontalScrollBar(), factor);
    adjustScrollBar(scrollArea->verticalScrollBar(), factor);

    zoomInAct->setEnabled(scaleFactor < 4.0);
    zoomOutAct->setEnabled(scaleFactor > 0.25);
}

void MainWindow::adjustScrollBar(QScrollBar *scrollBar, double factor)
{
    scrollBar->setValue(int(factor * scrollBar->value()
                            + ((factor - 1) * scrollBar->pageStep()/2)));
}
