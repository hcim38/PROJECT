#ifndef MAPEDITTOOLS_H
#define MAPEDITTOOLS_H

#include "GameItself/mapEditorMode.h"
#include "premademapspaths.h"

namespace Ui {
class mapEditTools;
}

class mapEditTools : public QWidget
{
    Q_OBJECT

public:
    explicit mapEditTools(MapEditor *Editor, QWidget *parent = nullptr);
    ~mapEditTools();

    MapEditor *editor;

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
