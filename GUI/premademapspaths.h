#ifndef PREMADEMAPSPATHS_H
#define PREMADEMAPSPATHS_H

#include <QDialog>

namespace Ui {
class PremadeMapsPaths;
}

class PremadeMapsPaths : public QDialog
{
    Q_OBJECT

public:
    explicit PremadeMapsPaths(QWidget *parent = nullptr);
    ~PremadeMapsPaths();

    void disableEmptybutt();

private slots:
    void on_buttEmptyMap_clicked();

    void on_buttMap1_clicked();
    void on_buttMap2_clicked();
    void on_buttMap3_clicked();
    void on_buttMap4_clicked();
    void on_buttMap5_clicked();
    void on_buttMap6_clicked();
    void on_buttMap7_clicked();
    void on_buttMap8_clicked();

    void on_buttMapMiki1_clicked();
    void on_buttMapMiki2_clicked();
    void on_buttMapMiki3_clicked();
    void on_buttMapMiki4_clicked();
    void on_buttMapMiki5_clicked();

private:
    Ui::PremadeMapsPaths *ui;

public:
    QString chosenFile;
};

#endif // PREMADEMAPSPATHS_H
