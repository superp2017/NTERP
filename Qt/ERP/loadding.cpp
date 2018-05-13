#include "loadding.h"
#include "ui_loadding.h"
#include <QDesktopWidget>
#include <QPoint>
#include <QPalette>
Loadding::Loadding(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Loadding)
{
    ui->setupUi(this);
    m_timer= new QTimer();
    connect(m_timer,SIGNAL(timeout()),this,SLOT(showOver()));
    setWindowFlags(Qt::FramelessWindowHint|Qt::SubWindow);//无边框
    setAttribute(Qt::WA_TranslucentBackground);//背景透明
    int frmX = width();
    int frmY = height();
    QDesktopWidget w;
    int deskWidth = w.width();
    int deskHeight = w.height();
    QPoint movePoint(deskWidth / 2 - frmX / 2, deskHeight / 2 - frmY / 2);
    move(movePoint);
    //加载gif图片
    movie = new QMovie(":/icon/loading.gif");
    ui->label_gif->setMovie(movie);
    movie->start();
}

Loadding::~Loadding()
{
    delete ui;
}

void Loadding::showTime(QString str, int sec, QColor c)
{
    showOver();
    movie->start();
    m_timer->start(sec);
    QPalette pe;
    pe.setColor(QPalette::WindowText,c);
    ui->label__content->setPalette(pe);
    ui->label__content->setText(str);
    this->exec();
}



void Loadding::showOver()
{
    if(m_timer->isActive()){
        m_timer->stop();
    }
    if(movie->isValid()){
        movie->stop();
    }
    if(!this->isHidden())
        this->done(0);
}


