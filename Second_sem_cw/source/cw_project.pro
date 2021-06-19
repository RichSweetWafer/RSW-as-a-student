QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

QMAKE_TARGET_COMPANY = Open Soure Student Projects by Zachary Samoilov
QMAKE_TARGET_PRODUCT = Qmage Editor
QMAKE_TARGET_DESCRIPTION = This project was made as a course task
QMAKE_TARGET_COPYRIGHT = Zachary Samoilov
RC_ICONS = Qmage_Editor.png
VERSION = 1.0


CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    image.cpp \
    imageviewer.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    image.h \
    imageviewer.h \
    mainwindow.h

FORMS += \
    mainwindow.ui



# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
