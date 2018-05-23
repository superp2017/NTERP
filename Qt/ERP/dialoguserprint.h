#ifndef DIALOGUSERPRINT_H
#define DIALOGUSERPRINT_H

#include <QDialog>
#include <QCheckBox>
#include "userservice.h"


namespace Ui {
class DialogUserPrint;
}

class DialogUserPrint : public QDialog
{
    Q_OBJECT

public:
    explicit DialogUserPrint(QWidget *parent = 0);
    ~DialogUserPrint();
    void initData(QVector<User> list);

private slots:
    void on_pushButton_cancle_clicked();

    void on_pushButton_print_clicked();

    void on_pushButton_export_clicked();

    void checkAll();

    void cellChecked(int row, int col);

    void removeAllRow();

    void setRowData(User user, int row);
    QVector<User> getSelectUsers();

    void exportCb(bool ok);
signals:
    void sig_exportCb(bool);
private:
    void doExport(QVector<User> ls, QString filepath);

    Ui::DialogUserPrint *ui;
    QVector<User>    m_employeess;
    QVector<QCheckBox*> m_checkboxs;

};

#endif // DIALOGUSERPRINT_H
