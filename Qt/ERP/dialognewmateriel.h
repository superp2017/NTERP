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

private slots:
    void on_pushButton_ok_clicked();

    void on_pushButton_cancle_clicked();

    void on_pushButton_add_custom_clicked();

    void on_pushButton_Add_color_clicked();

    void on_pushButton_add_type_clicked();

    void on_pushButton_clear_clicked();

private:
    void initCommbox(QVector<Customer> cus);

    Ui::DialogNewMateriel *ui;
    Materiel mater;
};

#endif // DIALOGNEWMATERIEL_H
