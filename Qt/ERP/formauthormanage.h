#ifndef FORMAUTHORMANAGE_H
#define FORMAUTHORMANAGE_H

#include <QWidget>

namespace Ui {
class FormAuthorManage;
}

class FormAuthorManage : public QWidget
{
    Q_OBJECT

public:
    explicit FormAuthorManage(QWidget *parent = 0);
    ~FormAuthorManage();
    void initData();

private:
    Ui::FormAuthorManage *ui;
};

#endif // FORMAUTHORMANAGE_H
