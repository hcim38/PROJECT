#include "mapedittools.h"
#include "ui_mapedittools.h"

mapEditTools::mapEditTools(MapEditor *Editor, QWidget *parent)
    : QWidget(parent), ui(new Ui::mapEditTools)
{
    setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowTitleHint);
    ui->setupUi(this);
    editor = Editor;
}

mapEditTools::~mapEditTools()
{
    delete ui;
}

void mapEditTools::on_buttExit_clicked()
{
    emit on_Exit();
    destroy();
}

void mapEditTools::on_buttNewTemplate_clicked()
{
    //TODO dialog 'u usure?'
    emit on_NewTemplate();
    ui->mapName->setText("MapName");
}

bool sortToFile(Tile const &one, Tile const &two)
{
    if (one.p_position.x == two.p_position.x) {
        return one.p_position.y < two.p_position.y;
    } else {
        return one.p_position.x < two.p_position.x;
    }
}

void mapEditTools::on_buttSave_clicked()
{
    std::vector<Tile> deleted = editor->getDeleted();
    if (!deleted.empty()) {
        std::sort(deleted.begin(), deleted.end(), sortToFile);

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

        QFile file(filename + "\\" + mapName + ".mapx");
        while (file.exists()) {
            file.setFileName(filename + "\\" + mapName + QString::number(i) + ".mapx");
            i++;
            if (i > 999) {
                break;
            }
        }
        if (!file.open(QIODevice::WriteOnly)) {
            //TODO dialog error & choose new place
        }
        QDataStream toFile(&file);
        for (auto &val : deleted) {
            toFile << val.position().x << val.position().y;
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

    QFile file(dialog.chosenFile());
    file.open(QFile::ReadOnly);
    if (file.isOpen()) {
        editor->generateTemplate();
        std::vector<sf::Vector2i> deleted;
        QDataStream in(&file);
        QString str;
        int x, y;

        while (!in.atEnd()) {
            in >> x >> y;
            deleted.emplace_back(sf::Vector2i(x, y));
        }
        file.close();
        editor->loadMap(deleted);
    }
}

void mapEditTools::on_buttLoadFile_clicked()
{
    QString appdata = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QString filename = QFileDialog::getOpenFileName(this,
                                                    tr("Open Map"),
                                                    appdata + "/maps",
                                                    tr("Map (*.mapx);;All files(*)"));
    if (filename == "")
        return;
    QFile file(filename);
    QString mapName = QFileInfo(file.fileName()).fileName();
    if (mapName.contains(".mapx")) {
        mapName.replace(".mapx", "");
    }
    ui->mapName->setText(mapName);
    file.open(QFile::ReadOnly);
    if (file.isOpen()) {
        editor->generateTemplate();
        std::vector<sf::Vector2i> deleted;
        QDataStream in(&file);
        QString str;
        int x, y;

        while (!in.atEnd()) {
            in >> x >> y;
            deleted.emplace_back(sf::Vector2i(x, y));
        }
        file.close();
        editor->loadMap(deleted);
    }
}
