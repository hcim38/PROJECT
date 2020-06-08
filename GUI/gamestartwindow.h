#ifndef GAMESTARTWINDOW_H
#define GAMESTARTWINDOW_H

#include "GameItself\game.h"
#include "GameItself\mapEditorMode.h"

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

    void captureRandomTiles(std::vector<Player> &players, Game &game);

private slots:
    void on_Player1Active_clicked();
    void on_Player2Active_clicked();
    void on_Player3Active_clicked();
    void on_Player4Active_clicked();
    void on_Player5Active_clicked();
    void on_startButton_clicked();

private:
    Ui::gameStartWindow *ui;
};

#endif // GAMESTARTWINDOW_H
