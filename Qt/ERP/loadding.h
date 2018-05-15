#ifndef LOADDING_H
#define LOADDING_H

#include <QDialog>
#include <QTimer>
#include <QMovie>
#include <QColor>

#pragma execution_character_set("utf-8")



namespace Ui {
class Loadding;
}

class Loadding : public QDialog
{
    Q_OBJECT

public:
    explicit Loadding(QWidget *parent = 0);
    ~Loadding();
    void showTime(QString str="",int sec=4000,QColor c =Qt::black );
public slots:
    void showOver();
private:
    Ui::Loadding *ui;
    QTimer *m_timer;
    QMovie *movie;
};

#endif // LOADDING_H
