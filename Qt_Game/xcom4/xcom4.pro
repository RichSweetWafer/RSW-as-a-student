QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Controls/controls.cpp \
    main.cpp \
    ConsoleLogStream/consolelogstream.cpp \
    Enemy/enemy.cpp \
    Field/field.cpp \
    Fieldbuilder/fieldbuilder.cpp \
    FileLogStream/filelogstream.cpp \
    Game/game.cpp \
    Item/item.cpp \
    Logger/logger.cpp \
    Object/object.cpp \
    Player/player.cpp \
    Printer/printer.cpp \
    Regulator/regulator.cpp \
    Tile/tile.cpp \
    widget.cpp

HEADERS += \
    ConsoleLogStream/consolelogstream.h \
    Controls/controls.h \
    Enemy/enemy.h \
    Field/field.h \
    Fieldbuilder/fieldbuilder.h \
    FileLogStream/filelogstream.h \
    Game/game.h \
    Item/item.h \
    LogStream/logstream.h \
    Logger/logger.h \
    Object/object.h \
    Player/player.h \
    Presenter/ipresenter.h \
    Presenter/presenter.h \
    Printer/printer.h \
    Regulator/regulator.h \
    Tile/tile.h \
    widget.h

FORMS += \
    widget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
