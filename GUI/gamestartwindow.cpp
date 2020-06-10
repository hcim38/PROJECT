#include "GUI\gamestartwindow.h"
#include "ui_gamestartwindow.h"

#include "startbuttonmapselection.h"

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

    //load map
    startButtonMapSelection dial(this);
    dial.setWindowFlag(Qt::WindowTitleHint);
    dial.show();

    dial.exec();

    if (dial.filename == "") {
        return;
    }

    QFile file(dial.filename);
    file.open(QFile::ReadOnly);
    if (file.isOpen()) {
        std::vector<Tile> newMap = game.generateTemplate(game.texture);
        std::vector<sf::Vector2i> deleted;
        QDataStream in(&file);
        QString str;
        int x, y;

        while (!in.atEnd()) {
            in >> x >> str >> y >> str;
            deleted.emplace_back(sf::Vector2i(x, y));
        }
        file.close();
        game.deletedTilesMapEdit.clear();
        for (auto const &pos : deleted) {
            for (auto tile = newMap.begin(); tile != newMap.end(); tile++) {
                if (pos.x == tile->m_position.x && pos.y == tile->m_position.y) {
                    tile->setColor(sf::Color(255, 0, 0, 100));
                    game.deletedTilesMapEdit.emplace_back(*tile);
                    newMap.erase(tile);
                    tile--;
                    continue;
                }
            }
        }
        game.MAP = newMap;
    }

    captureRandomTiles(TEMP, game);
    hide();
    game.players = TEMP;

    game.run();
    show();
}

void gameStartWindow::on_defaultButton_clicked()
{
    ui->groupBox_1->setEnabled(1);
    ui->playerHuman1->setChecked(1);
    ui->playerComputer1->setChecked(0);
    ui->playerNickname1->setText("Player 01");
    ui->groupBox_2->setEnabled(1);
    ui->playerHuman2->setChecked(0);
    ui->playerComputer2->setChecked(1);
    ui->playerNickname2->setText("Player 02");
    ui->groupBox_3->setEnabled(1);
    ui->playerHuman3->setChecked(0);
    ui->playerComputer3->setChecked(1);
    ui->playerNickname3->setText("Player 03");
    ui->groupBox_4->setEnabled(0);
    ui->playerHuman4->setChecked(0);
    ui->playerComputer4->setChecked(1);
    ui->playerNickname4->setText("Player 04");
    ui->groupBox_5->setEnabled(0);
    ui->playerHuman5->setChecked(0);
    ui->playerComputer5->setChecked(1);
    ui->playerNickname5->setText("Player 05");
}

void gameStartWindow::on_mapEditorButton_clicked()
{
    Game game;
    sf::Color temp;
    game.MAP = game.generateTemplate(game.texture);
    for (auto &val : game.MAP) {
        temp = val.getColor();
        temp.a = 255;
        val.setColor(temp);
    }
    hide();
    mapeditor(game);
    show();
}
