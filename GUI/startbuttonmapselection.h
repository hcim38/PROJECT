#ifndef STARTBUTTONMAPSELECTION_H
#define STARTBUTTONMAPSELECTION_H

#include <QDialog>
#include <QFileDialog>
#include <QStandardPaths>

namespace Ui {
class startButtonMapSelection;
}

class startButtonMapSelection : public QDialog
{
    Q_OBJECT

public:
    explicit startButtonMapSelection(QWidget *parent = nullptr);
    ~startButtonMapSelection();

    QString filename();

private slots:
    void on_buttPremade_clicked();

    void on_buttQuick_clicked();

    void on_buttCustom_clicked();

private:
    Ui::startButtonMapSelection *ui;
    QString p_filename;

public:
};

#endif // STARTBUTTONMAPSELECTION_H
