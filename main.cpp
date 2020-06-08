#include "GUI\gamestartwindow.h"
#include "GameItself\game.h"
#include "GameItself\gameMain.h"
#include "GameItself\mapEditorMode.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    gameStartWindow w;
    w.show();
    a.exec();
    return 1;
}
