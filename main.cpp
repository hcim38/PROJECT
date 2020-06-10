#include "GUI\gamestartwindow.h"
#include "GameItself\game.h"
#include "GameItself\mapEditorMode.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setApplicationName("Tile Conqueror");
    gameStartWindow w;
    w.show();
    a.exec();
    return 1;
}
