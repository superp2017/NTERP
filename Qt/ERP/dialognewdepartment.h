#ifndef DIALOGNEWDEPARTMENT_H
#define DIALOGNEWDEPARTMENT_H

#include <QDialog>

namespace Ui {
class DialogNewDepartment;
}

class DialogNewDepartment : public QDialog
{
    Q_OBJECT

public:
    explicit DialogNewDepartment(QWidget *parent = 0);
    ~DialogNewDepartment();

    QString getCur_department() const;

private slots:
    void on_pushButton_cancle_clicked();

    void on_pushButton__ok_clicked();

    void newDepartMentCb(QString de, bool ok);

private:
    Ui::DialogNewDepartment *ui;
    QString cur_department;
};

#endif // DIALOGNEWDEPARTMENT_H
