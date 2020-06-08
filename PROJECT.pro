QT += core
QT += widgets
TEMPLATE = app
CONFIG += c++17



SOURCES += \
        Banner.cpp \
        game.cpp \
        gameMain.cpp \
        gamestartwindow.cpp \
        main.cpp \
        mapEditorMode.cpp \
        player.cpp \
        tile.cpp

HEADERS += \
    banner.h \
    game.h \
    gameMain.h \
    gamestartwindow.h \
    mapEditorMode.h \
    player.h \
    tile.h

INCLUDEPATH += "C:/SFML-2.5.1/include"
LIBS += -L"C:/SFML-2.5.1/lib"
CONFIG(debug, debug|release){
    LIBS += -lsfml-audio-d -lsfml-graphics-d -lsfml-network-d -lsfml-system-d -lsfml-window-d
} else {
    LIBS += -lsfml-audio -lsfml-graphics -lsfml-network -lsfml-system -lsfml-window
}

RESOURCES += \
    Resources.qrc

FORMS += \
    gamestartwindow.ui

