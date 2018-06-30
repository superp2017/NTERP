#ifndef DIALOGNEWMATERIEL_H
#define DIALOGNEWMATERIEL_H

#include <QDialog>
#include "service_global.h"

#pragma execution_character_set("utf-8")


namespace Ui {
class DialogNewMateriel;
}

class DialogNewMateriel : public QDialog
{
    Q_OBJECT

public:
    explicit DialogNewMateriel(QWidget *parent = 0);
    ~DialogNewMateriel();

    Materiel getMater() const;
    void initCommbox();

private slots:
    void on_pushButton_ok_clicked();

    void on_pushButton_cancle_clicked();

    void on_pushButton_clear_clicked();

    void DesChange();

    void on_comboBox_solid_currentTextChanged(const QString &arg1);

    void on_comboBox_format_currentTextChanged(const QString &arg1);

private:
    Ui::DialogNewMateriel *ui;
    Materiel mater;
     QVector<Materiel> mater_list;

};

#endif // DIALOGNEWMATERIEL_H
