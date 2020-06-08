#include "game.h"
#include "gameMain.h"
#include "gamestartwindow.h"
#include "mapEditorMode.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    gameStartWindow w;
    w.show();
    a.exec();
    return 1;
}
