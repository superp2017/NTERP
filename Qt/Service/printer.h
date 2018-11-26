#ifndef PRINTER_H
#define PRINTER_H

#include <QVector>
#include <QString>
#include <QPrinter>
#include <QObject>
#include "service_global.h"
#pragma execution_character_set("utf-8")

class SERVICESHARED_EXPORT printer : public  QObject
{
    Q_OBJECT
public:
    printer(bool isTwoTtile =true, \
            QString firstTitle=QString("海门市江淤电镀有限公司"), \
            QString secTtile=QString("南通江淤新材料有限公司"), QObject *w=0);
    printer(QVector<Order> d,QString printNum,bool isTwoTtile =true,\
            QString firstTitle=QString("海门市江淤电镀有限公司"),\
            QString secTtile=QString("南通江淤新材料有限公司"),QObject *w=0);
    void doPreview(QWidget *w);
    void doPrint(QWidget* w);

    void setIsTwoTtile(bool isTwoTtile);

    void setFistTitle(const QString &fistTitle);

    void setSecTtile(const QString &secTtile);

    void setData(const QVector<Order> &data,QString printNum);
private slots:
    void printDocument(QPrinter *printer);

signals:
    void updateOrderPrintNum(QVector<Order> order);
private:

    void printRow(QPainter *print, QVector<QString>& data, \
                  int row, double left, double top, \
                  double d_w,double d_h);

private:
    QVector<Order> m_data;
    bool m_isTwoTtile;
    QString m_fistTitle;
    QString m_secTtile;
    QString m_printNum;
};

#endif // PRINTER_H
