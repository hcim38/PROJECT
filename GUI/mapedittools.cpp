#include "mapedittools.h"
#include "ui_mapedittools.h"

mapEditTools::mapEditTools(QWidget *parent) : QWidget(parent), ui(new Ui::mapEditTools)
{
    setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowTitleHint);
    ui->setupUi(this);
}

mapEditTools::~mapEditTools()
{
    delete ui;
}

void mapEditTools::on_buttExit_clicked()
{
    //TODO dialog 'u sure?'
    game->killTheMapEdit = 1;
    destroy();
}

void mapEditTools::on_buttNewTemplate_clicked()
{
    //TODO dialog 'u usure?'
    game->newTemplateMapEdit = 1;
    ui->mapName->setText("MapName");
}

bool sortToFile(Tile const &one, Tile const &two)
{
    if (one.m_position.x == two.m_position.x) {
        return one.m_position.y < two.m_position.y;
    } else {
        return one.m_position.x < two.m_position.x;
    }
}

void mapEditTools::on_buttSave_clicked()
{
    if (!game->deletedTilesMapEdit.empty()) {
        std::sort(game->deletedTilesMapEdit.begin(), game->deletedTilesMapEdit.end(), sortToFile);

        QString filename = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
        QDir dir = filename;
        if (!dir.exists()) {
            dir.mkpath(filename);
        }
        if (!dir.cd("maps")) {
            dir.mkdir("maps");
            dir.cd("maps");
        }
        filename = dir.absolutePath();

        int i = 0;
        QString mapName = ui->mapName->text();
        for (auto &val : mapName) {
            if (val == ' ') {
                val = '_';
            }
        }

        QFile file(filename + "\\" + mapName + ".map");
        while (file.exists()) {
            file.setFileName(filename + "\\" + mapName + QString::number(i) + ".map");
            i++;
            if (i > 999) {
                break;
            }
        }
        if (!file.open(QIODevice::WriteOnly)) {
            //TODO dialog error & choose new place
        }
        QDataStream toFile(&file);
        for (auto const &val : game->deletedTilesMapEdit) {
            toFile << val.m_position.x << "|" << val.m_position.y << ";";
        }
        file.close();
        //TODO dialog saved as filename
    } else {
        //TODO dialog 'nothing to save!(template)'
    }
}

void mapEditTools::on_buttLoad_clicked()
{
    PremadeMapsPaths dialog(this);
    dialog.setWindowFlag(Qt::Tool);

    dialog.show();
    dialog.exec();

    QFile file(dialog.chosenFile);
    file.open(QFile::ReadOnly);
    if (file.isOpen()) {
        game->MAP = game->generateTemplate(game->texture);
        std::vector<sf::Vector2i> deleted;
        QDataStream in(&file);
        QString str;
        int x, y;

        while (!in.atEnd()) {
            in >> x >> str >> y >> str;
            deleted.emplace_back(sf::Vector2i(x, y));
        }
        file.close();
        game->deletedTilesMapEdit.clear();
        for (auto const &pos : deleted) {
            for (auto tile = game->MAP.begin(); tile != game->MAP.end(); tile++) {
                if (pos.x == tile->m_position.x && pos.y == tile->m_position.y) {
                    tile->setColor(sf::Color(255, 0, 0, 100));
                    game->deletedTilesMapEdit.emplace_back(*tile);
                    game->MAP.erase(tile);
                    tile--;
                    continue;
                }
            }
        }
    }
}

void mapEditTools::on_buttLoadFile_clicked()
{
    QString appdata = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QString filename = QFileDialog::getOpenFileName(this,
                                                    tr("Open Map"),
                                                    appdata + "/maps",
                                                    tr("Map (*.map);;All files(*)"));
    if (filename == "")
        return;
    QFile file(filename);
    QString mapName = QFileInfo(file.fileName()).fileName();
    if (mapName.contains(".map")) {
        mapName.replace(".map", "");
    }
    ui->mapName->setText(mapName);
    file.open(QFile::ReadOnly);
    if (file.isOpen()) {
        std::vector<Tile> newMap = game->generateTemplate(game->texture);
        std::vector<sf::Vector2i> deleted;
        QDataStream in(&file);
        QString str;
        int x, y;

        while (!in.atEnd()) {
            in >> x >> str >> y >> str;
            deleted.emplace_back(sf::Vector2i(x, y));
        }
        file.close();
        game->deletedTilesMapEdit.clear();
        for (auto const &pos : deleted) {
            for (auto tile = newMap.begin(); tile != newMap.end(); tile++) {
                if (pos.x == tile->m_position.x && pos.y == tile->m_position.y) {
                    tile->setColor(sf::Color(255, 0, 0, 100));
                    game->deletedTilesMapEdit.emplace_back(*tile);
                    newMap.erase(tile);
                    continue;
                }
            }
        }
        game->MAP = newMap;
    }
}
