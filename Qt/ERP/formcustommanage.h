#ifndef FORMCUSTOMMANAGE_H
#define FORMCUSTOMMANAGE_H

#include <QWidget>

namespace Ui {
class FormCustommanage;
}

class FormCustommanage : public QWidget
{
    Q_OBJECT

public:
    explicit FormCustommanage(QWidget *parent = 0);
    ~FormCustommanage();

private slots:
    void on_pushButton_del_clicked();

    void on_pushButton_mod_clicked();

    void on_pushButton_exit_clicked();

private:
    Ui::FormCustommanage *ui;
};

#endif // FORMCUSTOMMANAGE_H
