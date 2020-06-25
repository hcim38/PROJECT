#include "startbuttonmapselection.h"
#include "GUI/premademapspaths.h"
#include "QRandomGenerator"
#include "ui_startbuttonmapselection.h"

startButtonMapSelection::startButtonMapSelection(QWidget *parent) :
      QDialog(parent),
      ui(new Ui::startButtonMapSelection)
{
    ui->setupUi(this);
}

startButtonMapSelection::~startButtonMapSelection()
{
    delete ui;
}

void startButtonMapSelection::on_buttPremade_clicked()
{
    PremadeMapsPaths dialog(this);
    dialog.setWindowFlag(Qt::Tool);
    dialog.show();
    dialog.disableEmptybutt();
    dialog.exec();
    p_filename = dialog.chosenFile();
    done(1);
}

void startButtonMapSelection::on_buttQuick_clicked()
{
    QRandomGenerator randomizer(QRandomGenerator::securelySeeded());
    int map = randomizer.bounded(0, 11);

    switch (map) {
    case 0:
        p_filename = ":/Maps/Resources/Mikado's_Map.map";
        break;
    case 1:
        p_filename = ":/Maps/Resources/Mikado's_Map2.map";
        break;
    case 2:
        p_filename = ":/Maps/Resources/Mikado's_Map3.map";
        break;
    case 3:
        p_filename = ":/Maps/Resources/Mikado's_Map4.map";
        break;
    case 4:
        p_filename = ":/Maps/Resources/Mikado's_Map5.map";
        break;
    case 5:
        p_filename = ":/Maps/Resources/NextRandMap.map";
        break;
    case 6:
        p_filename = ":/Maps/Resources/noIdeaWhatIveCreated.map";
        break;
    case 7:
        p_filename = ":/Maps/Resources/randMap1.map";
        break;
    case 8:
        p_filename = ":/Maps/Resources/randMap2.map";
        break;
    case 9:
        p_filename = ":/Maps/Resources/randMap3.map";
        break;
    case 10:
        p_filename = ":/Maps/Resources/Snek.map";
        break;
    case 11:
        p_filename = ":/Maps/Resources/spirala.map";
        break;
    }
    done(1);
}

void startButtonMapSelection::on_buttCustom_clicked()
{
    QString appdata = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    p_filename = QFileDialog::getOpenFileName(this,
                                              tr("Open Map"),
                                              appdata + "/maps",
                                              tr("Map (*.map);;All files(*)"));
    done(1);
}

QString startButtonMapSelection::filename()
{
    return p_filename;
}
