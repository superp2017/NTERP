#ifndef DIALOGORDERSEARCH_H
#define DIALOGORDERSEARCH_H

#include <QDialog>
#include <QMap>
#pragma execution_character_set("utf-8")

namespace Ui {
class DialogOrderSearch;
}


class DialogOrderSearch : public QDialog
{
    Q_OBJECT

public:
    explicit DialogOrderSearch(QWidget *parent = 0);
    ~DialogOrderSearch();
    void initSearchContent(QMap<QString,QVector<QString>> data);
signals:
    void searchOrder(bool isTime,bool isOther,qint64 min,qint64 max,QString type,QString content);
    void showAll();
private slots:
    void on_pushButton_ok_clicked();
    void on_pushButton_exit_clicked();
    void changeUIEnable();
    void changeContent(QString type);
    void on_pushButton_showall_clicked();

private:
    void setenable(bool time, bool other);
    Ui::DialogOrderSearch *ui;
    QMap<QString,QVector<QString>>  m_search;
};

#endif // DIALOGORDERSEARCH_H
