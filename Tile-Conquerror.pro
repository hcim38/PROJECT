QT += core
QT += widgets
TEMPLATE = app
CONFIG += c++17



SOURCES += \
        GameItself\Banner.cpp \
        GameItself\game.cpp \
        GUI\gamestartwindow.cpp \
        GameItself\functionsOutsideClasses.cpp \
        main.cpp \
        GameItself\mapEditorMode.cpp \
        GameItself\player.cpp \
        GameItself\tile.cpp \
        GUI\mapedittools.cpp \
        GUI\premademapspaths.cpp \
        GUI\startbuttonmapselection.cpp

HEADERS += \
    GameItself\banner.h \
    GameItself\game.h \
    GUI\gamestartwindow.h \
    GameItself\mapEditorMode.h \
    GameItself\player.h \
    GameItself\tile.h \
    GUI\mapedittools.h \
    GUI\premademapspaths.h \
    GUI\startbuttonmapselection.h \
    GameItself\functionsOutsideClasses.h

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
    GUI\gamestartwindow.ui \
    GUI\mapedittools.ui \
    GUI\premademapspaths.ui \
    GUI\startbuttonmapselection.ui

