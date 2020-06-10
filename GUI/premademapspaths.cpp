#include "premademapspaths.h"
#include "ui_premademapspaths.h"

PremadeMapsPaths::PremadeMapsPaths(QWidget *parent) : QDialog(parent), ui(new Ui::PremadeMapsPaths)
{   
    ui->setupUi(this);
}

PremadeMapsPaths::~PremadeMapsPaths()
{
    delete ui;
}

void PremadeMapsPaths::disableEmptybutt()
{
    ui->buttEmptyMap->hide();
}

void PremadeMapsPaths::on_buttEmptyMap_clicked()
{
    chosenFile = ":/Maps/Resources/emptyTemplate.map";
    done(1);
}

void PremadeMapsPaths::on_buttMap1_clicked()
{
    chosenFile = ":/Maps/Resources/randMap1.map";
    done(1);
}
void PremadeMapsPaths::on_buttMap2_clicked()
{
    chosenFile = ":/Maps/Resources/randMap2.map";
    done(1);
}
void PremadeMapsPaths::on_buttMap3_clicked()
{
    chosenFile = ":/Maps/Resources/randMap3.map";
    done(1);
}
void PremadeMapsPaths::on_buttMap4_clicked()
{
    chosenFile = ":/Maps/Resources/noIdeaWhatIveCreated.map";
    done(1);
}
void PremadeMapsPaths::on_buttMap5_clicked()
{
    chosenFile = ":/Maps/Resources/NextRandMap.map";
    done(1);
}
void PremadeMapsPaths::on_buttMap6_clicked()
{
    chosenFile = ":/Maps/Resources/spirala.map";
    done(1);
}
void PremadeMapsPaths::on_buttMap7_clicked()
{
    chosenFile = ":/Maps/Resources/Snek.map";
    done(1);
}
void PremadeMapsPaths::on_buttMap8_clicked()
{
    chosenFile = ":/Maps/Resources/TCHC.map";
    done(1);
}

void PremadeMapsPaths::on_buttMapMiki1_clicked()
{
    chosenFile = ":/Maps/Resources/Mikado's_Map.map";
    done(1);
}

void PremadeMapsPaths::on_buttMapMiki2_clicked()
{
    chosenFile = ":/Maps/Resources/Mikado's_Map2.map";
    done(1);
}

void PremadeMapsPaths::on_buttMapMiki3_clicked()
{
    chosenFile = ":/Maps/Resources/Mikado's_Map3.map";
    done(1);
}

void PremadeMapsPaths::on_buttMapMiki4_clicked()
{
    chosenFile = ":/Maps/Resources/Mikado's_Map4.map";
    done(1);
}

void PremadeMapsPaths::on_buttMapMiki5_clicked()
{
    chosenFile = ":/Maps/Resources/Mikado's_Map5.map";
    done(1);
}
