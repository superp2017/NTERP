#ifndef DIALOGNEWORDER_H
#define DIALOGNEWORDER_H

#include <QDialog>
#include "global.h"
#include <QJsonObject>

#pragma execution_character_set("utf-8")

namespace Ui {
class DialogNewOrder;
}


class DialogNewOrder : public QDialog
{
    Q_OBJECT

public:
    explicit DialogNewOrder( QWidget *parent = 0);
    ~DialogNewOrder();
    void  initOrder(Order order);
    void  setModel(bool isNew);
    Order getCurorder() const;
private slots:
    void on_pushButton_ok_clicked();
    void on_pushButton_cancel_clicked();
    void newOrderCb(Order order, bool ok);
    void modOrderCb(Order order,bool ok);
    void companyNameChange(int index);
    void companyNameChange(QString id);
    void materielIDChange(int index);
private:
    void initCombox(QSet<QString> batch,QVector<Materiel>mater);
    bool checkOrder(Order curorder, bool ismod);
    void changeModel();
    void clearCurMater();
    void setCurMater();
    Ui::DialogNewOrder *ui;
    Order    curorder;
    bool     m_isNewMode;//是否是新建或者修改
    Materiel curMater;
    QMap<QString,QVector<QString>> m_company_mater;
};

#endif // DIALOGNEWORDER_H
