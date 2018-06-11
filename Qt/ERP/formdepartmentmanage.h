#ifndef FORMDEPARTMENTMANAGE_H
#define FORMDEPARTMENTMANAGE_H

#include <QWidget>
#include <QListWidgetItem>

namespace Ui {
class FormDepartmentManage;
}

class FormDepartmentManage : public QWidget
{
    Q_OBJECT

public:
    explicit FormDepartmentManage(QWidget *parent = 0);
    ~FormDepartmentManage();
    void initData();

private slots:
    void on_pushButton_add_clicked();

    void on_pushButton_del_clicked();

    void on_pushButton_exit_clicked();

    void newDepartment(QString de,bool ok);
    void delDepartment(QString de,bool ok);

    void itemClicked(QListWidgetItem *item);
private:
    Ui::FormDepartmentManage *ui;
     QString curDepartment;
};

#endif // FORMDEPARTMENTMANAGE_H
