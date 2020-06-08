#include "GUI\gamestartwindow.h"
#include "ui_gamestartwindow.h"

#include "GameItself\gameMain.h"

gameStartWindow::gameStartWindow(QWidget *parent) :
      QWidget(parent),
      ui(new Ui::gameStartWindow)
{
    ui->setupUi(this);
}

gameStartWindow::~gameStartWindow()
{
    delete ui;
}

void gameStartWindow::captureRandomTiles(std::vector<Player> &players, Game &game)
{
    game.MAP = game.generateTemplate(game.texture, sf::Vector2(30, 30), 10);
    players[0].p_ownership = game.MAP;

    QRandomGenerator randomizer(QRandomGenerator::securelySeeded());
    QString playerName;

    for (auto it = players.begin() + 1; it != players.end(); it++) {
        game.capture(players[0]
                         .ownership()[randomizer.bounded(0, (players[0].p_ownership.size() - 1))],
                     players[0],
                     *it);
    }
    for (auto it = players.begin() + 1; it != players.end(); it++) {
        it->p_ownership[0].setBegginerValue();
    }
}

void gameStartWindow::on_Player1Active_clicked()
{
    if (ui->Player1Active->checkState()) {
        ui->groupBox_1->setEnabled(true);
    } else {
        ui->groupBox_1->setEnabled(false);
    }
}

void gameStartWindow::on_Player2Active_clicked()
{
    if (ui->Player2Active->checkState()) {
        ui->groupBox_2->setEnabled(true);
    } else {
        ui->groupBox_2->setEnabled(false);
    }
}

void gameStartWindow::on_Player3Active_clicked()
{
    if (ui->Player3Active->checkState()) {
        ui->groupBox_3->setEnabled(true);
    } else {
        ui->groupBox_3->setEnabled(false);
    }
}

void gameStartWindow::on_Player4Active_clicked()
{
    if (ui->Player4Active->checkState()) {
        ui->groupBox_4->setEnabled(true);
    } else {
        ui->groupBox_4->setEnabled(false);
    }
}

void gameStartWindow::on_Player5Active_clicked()
{
    if (ui->Player5Active->checkState()) {
        ui->groupBox_5->setEnabled(true);
    } else {
        ui->groupBox_5->setEnabled(false);
    }
}

void gameStartWindow::on_startButton_clicked()
{
    Game game;
    std::vector<Player> TEMP;
    TEMP.emplace_back(Player("MAP", 0));
    int i = 1;
    if (ui->groupBox_1->isEnabled()) {
        TEMP.emplace_back(
            Player((ui->playerNickname1->text()), i, ui->playerComputer1->isChecked()));
        i++;
    }
    if (ui->groupBox_2->isEnabled()) {
        TEMP.emplace_back(
            Player((ui->playerNickname2->text()), i, ui->playerComputer2->isChecked()));
        i++;
    }
    if (ui->groupBox_3->isEnabled()) {
        TEMP.emplace_back(
            Player((ui->playerNickname3->text()), i, ui->playerComputer3->isChecked()));
        i++;
    }
    if (ui->groupBox_4->isEnabled()) {
        TEMP.emplace_back(
            Player((ui->playerNickname4->text()), i, ui->playerComputer4->isChecked()));
        i++;
    }
    if (ui->groupBox_5->isEnabled()) {
        TEMP.emplace_back(
            Player((ui->playerNickname5->text()), i, ui->playerComputer5->isChecked()));
        i++;
    }

    captureRandomTiles(TEMP, game);
    game.players = TEMP;
    hide();
    game1(game);
    show();
}
