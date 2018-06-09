#ifndef DIALOGNEWGOODSTYPE_H
#define DIALOGNEWGOODSTYPE_H

#include <QDialog>
#pragma execution_character_set("utf-8")

namespace Ui {
class DialogNewGoodsType;
}

class DialogNewGoodsType : public QDialog
{
    Q_OBJECT

public:
    explicit DialogNewGoodsType(QWidget *parent = 0);
    ~DialogNewGoodsType();
    QString getType()const;

private slots:

    void on_pushButton_cancle_clicked();

    void on_pushButton__ok_clicked();

    void newTypeCb(QString type,bool ok);
private:
    Ui::DialogNewGoodsType *ui;
    QString curType;
};

#endif // DIALOGNEWGOODSTYPE_H
