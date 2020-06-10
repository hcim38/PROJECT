#ifndef MAPEDITTOOLS_H
#define MAPEDITTOOLS_H

#include "GameItself/game.h"
#include <QWidget>

namespace Ui {
class mapEditTools;
}

class mapEditTools : public QWidget
{
    Q_OBJECT

public:
    explicit mapEditTools(QWidget *parent = nullptr);
    ~mapEditTools();

    Game *game;

private slots:
    void on_buttExit_clicked();

    void on_buttNewTemplate_clicked();

    void on_buttSave_clicked();

    void on_buttLoad_clicked();

    void on_buttLoadFile_clicked();

private:
    Ui::mapEditTools *ui;
};

#endif // MAPEDITTOOLS_H
