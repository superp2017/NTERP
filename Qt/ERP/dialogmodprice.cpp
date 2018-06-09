#include "dialogmodprice.h"
#include "ui_dialogmodprice.h"
#include "datacenter.h"
#include <QToolTip>
#include "orderservice.h"
#include "boost/thread.hpp"

DialogModPrice::DialogModPrice(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogModPrice)
{
    ui->setupUi(this);
    connect(dataCenter::instance(),SIGNAL(sig_modOrderPrice(Order,bool)),this,SLOT(modOrderPriceCb(Order,bool)));
}

DialogModPrice::~DialogModPrice()
{
    delete ui;
}

void DialogModPrice::initData(Order order)
{
    curOrder = order;
    ui->lineEdit_orderID->setText(order.OrderID);
    ui->lineEdit_price->setText(QString("%1").arg( order.Money/100.0));
}

void DialogModPrice::on_pushButton_ok_clicked()
{
    int money = ui->lineEdit_price->text().toInt()*100;
    if(money<=0){
        QToolTip::showText(ui->lineEdit_price->mapToGlobal(QPoint(100, 0)), "订单价格填写错误!");
        return ;
    }
    curOrder.Money = money;
    boost::thread t(boost::bind(&dataCenter::net_modOrderPrice,dataCenter::instance(),OrderService::toJsonObject(curOrder)));
    t.detach();
    dataCenter::instance()->pub_showLoadding("正在网络请求...",5000,Qt::black);

}

void DialogModPrice::on_pushButton__cancle_clicked()
{
    done(-1);
}

void DialogModPrice::modOrderPriceCb(Order order, bool ok)
{
    dataCenter::instance()->pub_hideLoadding();
    if(ok){
        dataCenter::instance()->pub_showMessage("改价成功!",4000);
        curOrder = order;
        done(123);
    }else{
        dataCenter::instance()->pub_showMessage("改价失败!",4000);
        curOrder = order;
        done(0);
    }
}

Order DialogModPrice::getCurOrder() const
{
    return curOrder;
}
