#include "printer.h"

#include <QPrinter>
#include <QPrintDialog>
#include <QPrintPreviewDialog>
#include <QPagedPaintDevice>
#include <QPainter>


printer::printer(bool isTwoTtile, QString firstTitle, QString secTtile,QObject *w):QObject(w)
{
    m_isTwoTtile   = isTwoTtile;
    m_fistTitle    = firstTitle;
    m_secTtile     = secTtile;
}

printer::printer(QVector<Order> d,QString printNum, bool isTwoTtile, \
                 QString firstTitle, QString secTtile,QObject *w):QObject(w)
{
    m_data         = d;
    m_isTwoTtile   = isTwoTtile;
    m_fistTitle    = firstTitle;
    m_secTtile     = secTtile;
    m_printNum     = printNum;
}

#include <QDebug>

void printer::doPreview(QWidget* w)
{
    if(m_data.size()==0) return;
    QPrinter printer(QPrinter::HighResolution);
    QPrintPreviewDialog preview(&printer, w);
    connect(&preview, SIGNAL(paintRequested(QPrinter*)),this, SLOT(printDocument(QPrinter*)));
    if (preview.exec()==QDialog::Accepted){
        emit  updateOrderPrintNum(m_data);
    }
}


void printer::doPrint(QWidget* w)
{
    if(m_data.size()==0) return;
    QPrinter printer(QPrinter::HighResolution);
    printer.setPageSize(QPagedPaintDevice::Custom);
    printer.setPageSizeMM(QSizeF(220,140));
    printer.setPageMargins(0,0,0,0,QPrinter::Millimeter);
    printer.setPageOrientation(QPageLayout::Portrait);
    QPrintDialog dialog(&printer, w);
    if (dialog.exec() == QDialog::Accepted)
    {
        printDocument(&printer);
        emit updateOrderPrintNum(m_data);
    }
}

void printer::printDocument(QPrinter *printer)
{
    QPainter painter;
    painter.begin(printer);
    painter.save();
    double left     = printer->width()/9.0;
    double right    = left*8;
    double top      = printer->height()*0.20;
    double bottom   = printer->height()/9.0*8;
    double width    = right-left;
    double height   = bottom-top;


    painter.setFont(QFont("宋体", 18, QFont::Normal));

    double title_h = printer->height()*0.05;
    if(m_isTwoTtile)
        painter.drawText(0,0,printer->width(),title_h,Qt::AlignCenter,m_fistTitle);
    painter.drawText(0,title_h,printer->width(),title_h,Qt::AlignCenter,m_secTtile);


    painter.setFont(QFont("宋体", 16, QFont::Normal));
    QString third_Title("来料加工送货单");


    double secTitle_bottom = printer->height()*0.10;

    double third_Titile_bottom = printer->height()*0.14;

    painter.drawText(0,secTitle_bottom+10,printer->width(),third_Titile_bottom-secTitle_bottom,Qt::AlignHCenter|Qt::AlignTop,third_Title);

    painter.setFont(QFont("宋体", 14, QFont::Normal));

    painter.drawText(left+30,third_Titile_bottom,printer->width()/7.0,top-third_Titile_bottom,Qt::AlignVCenter|Qt::AlignLeft,"客户名称：");


    QString customerName = m_data.at(0).CustomName;

    painter.drawText(left*3,third_Titile_bottom,printer->width()/7.0*3,top-third_Titile_bottom,Qt::AlignVCenter|Qt::AlignLeft,customerName);

    painter.setFont(QFont("宋体", 16, QFont::Normal));
    QString number = QString("N.O ")+m_printNum;
    painter.drawText(left,third_Titile_bottom,right-left,top-third_Titile_bottom,Qt::AlignVCenter|Qt::AlignRight,number);

    painter.setFont(QFont("宋体", 11, QFont::Normal));
    painter.drawRect(left,top,width,height);

    double d_w = width/30.0;
    double d_h = height/11.0;

    for(int i =1;i<30;i++){
        if(i==2||i==8||i==20||i==23||i==25)
            painter.drawLine(left+i*d_w,top,left+i*d_w,bottom);
    }

    for(int i =1;i<11;i++){
        painter.drawLine(left,top+i*d_h,right,top+i*d_h);
    }


    QVector<QString> title;
    title.push_back("行号");
    title.push_back("生产批号");
    title.push_back("物料描述");
    title.push_back("数量");
    title.push_back("单位");
    title.push_back("备注");

    printRow(&painter,title,0,left,top,d_w,d_h);

    painter.setFont(QFont("宋体", 11, QFont::Normal));


    for (int i=0;i<m_data.size();++i){
        QVector<QString> test;
        Order o   = m_data.at(i);
        test.push_back(QString("%1").arg(i+1));
        test.push_back(o.OrderID);
        test.push_back(o.ComponentSolid+"/"+o.ComponentFormat+"/"+o.Plating);
        test.push_back(QString("%1").arg(o.SuccessNum));
        test.push_back(o.Unit);
        test.push_back(o.CustomNote);
        printRow(&painter,test,1+i,left,top,d_w,d_h);
    }

    painter.drawText(printer->width()/5.0, printer->height()/9.0*8+80,"送货人：");
    painter.drawText(printer->width()/2.0, printer->height()/9.0*8+80,"客户签收：");
    painter.drawText(printer->width()/5.0*4, printer->height()/9.0*8+80,"日期：");



    //Restores the current painter state (pops a saved state off the stack).
    painter.restore();

    painter.end();
}

void printer::printRow(QPainter *print, QVector<QString> &data, int row, double left, double top, double d_w, double d_h)
{
    for(int i=0;i<data.size();i++){
        double width = d_w;
        double l = left;
        if(row==0||i==0){
            print->setFont(QFont("宋体", 15, QFont::Normal));
        }else{
            print->setFont(QFont("宋体", 11, QFont::Normal));
        }
        if(i==0) {
            width =d_w*2;
            l =left;
        }
        if(i==1) {
            width =d_w*6;
            l =left+d_w*2;
        }
        if(i==2) {
            width =d_w*12;
            l =left+d_w*8;
        }
        if(i==3) {
            width =d_w*3;
            l =left+d_w*20;
        }
        if(i==4) {
            width =d_w*2;
            l =left+d_w*23;
        }
        if(i==5) {
            width =d_w*5;
            l =left+d_w*25;
        }

        int flag = Qt::TextWrapAnywhere|Qt::AlignCenter;
        print->drawText(l,top+row*d_h,width,d_h,flag,data.at(i));
    }
}



void printer::setData(const QVector<Order> &data,QString printNum)
{
    m_data = data;
    m_printNum = printNum;
}


void printer::setSecTtile(const QString &secTtile)
{
    m_secTtile = secTtile;
}

void printer::setFistTitle(const QString &fistTitle)
{
    m_fistTitle = fistTitle;
}

void printer::setIsTwoTtile(bool isTwoTtile)
{
    m_isTwoTtile = isTwoTtile;
}

