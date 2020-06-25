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

public slots:
    void on_buttExit_clicked();

    void on_buttNewTemplate_clicked();

signals:
    void on_Exit();

    void on_NewTemplate();

private slots:
    void on_buttSave_clicked();

    void on_buttLoad_clicked();

    void on_buttLoadFile_clicked();

private:
    Ui::mapEditTools *ui;
    MapEditor *editor;
};

#endif // MAPEDITTOOLS_H
