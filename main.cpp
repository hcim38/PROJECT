#include "GUI/gamestartwindow.h"
#include <QApplication>

//TODO add distribution of forces bar to Banner
//TODO add save game progress
//TODO add pause menu GUI
//TODO highscores?
//TODO add randomization factor to fights?

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setApplicationName("Tile Conqueror");
    gameStartWindow w;
    w.show();
    a.exec();
    return 1;
}
