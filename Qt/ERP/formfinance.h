#ifndef FORMFINANCE_H
#define FORMFINANCE_H

#include <QWidget>

namespace Ui {
class FormFinance;
}

class FormFinance : public QWidget
{
    Q_OBJECT

public:
    explicit FormFinance(QWidget *parent = 0);
    ~FormFinance();

private slots:
    void on_pushButton_search_clicked();

    void on_pushButton_export_clicked();

private:
    Ui::FormFinance *ui;
};

#endif // FORMFINANCE_H
