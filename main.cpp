#include "GUI\gamestartwindow.h"
#include "GameItself\game.h"
#include "GameItself\mapEditorMode.h"
#include <QApplication>

//TODO add win condition
//TODO after pressing start show available maps or choose from file

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setApplicationName("Tile Conqueror");
    gameStartWindow w;
    w.show();
    a.exec();
    return 1;
}
