#ifndef GAMESTARTWINDOW_H
#define GAMESTARTWINDOW_H

#include "game.h"
#include "mapEditorMode.h"

#include <QWidget>

namespace Ui {
class gameStartWindow;
}

class gameStartWindow : public QWidget
{
    Q_OBJECT

public:
    explicit gameStartWindow(QWidget *parent = nullptr);
    ~gameStartWindow();

    std::vector<Player> players;
    void captureRandomTiles(std::vector<Player> &players);

private slots:
    void on_Player1Active_clicked();
    void on_Player2Active_clicked();
    void on_Player3Active_clicked();
    void on_Player4Active_clicked();
    void on_Player5Active_clicked();
    void on_startButton_clicked();

private:
    Ui::gameStartWindow *ui;
    Game game;
};

#endif // GAMESTARTWINDOW_H
