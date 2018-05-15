#ifndef DIALOGNEWUNIT_H
#define DIALOGNEWUNIT_H

#include <QDialog>
#pragma execution_character_set("utf-8")


namespace Ui {
class DialogNewUnit;
}

class DialogNewUnit : public QDialog
{
    Q_OBJECT

public:
    explicit DialogNewUnit(QWidget *parent = 0);
    ~DialogNewUnit();

    QString getUnit() const;

private slots:
    void on_pushButton__ok_clicked();

    void on_pushButton_cancle_clicked();

private:
    Ui::DialogNewUnit *ui;
    QString cur_unit;
};

#endif // DIALOGNEWUNIT_H
