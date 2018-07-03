#ifndef DIALOGORDERPRODUCEOROUT_H
#define DIALOGORDERPRODUCEOROUT_H

#include <QDialog>

namespace Ui {
class DialogOrderProduceOrOut;
}

class DialogOrderProduceOrOut : public QDialog
{
    Q_OBJECT

public:
    explicit DialogOrderProduceOrOut(QWidget *parent = 0);
    ~DialogOrderProduceOrOut();
    void initData(bool isSuccess, QString OrderID, double un, QString unit);
private slots:
    void on_pushButton_ok_clicked();

    void on_pushButton_exit_clicked();

private:
    Ui::DialogOrderProduceOrOut *ui;
    QString curOrderID ;
    bool m_isSuccess;

};

#endif // DIALOGORDERPRODUCEOROUT_H
