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

private:
    Ui::FormAuthorManage *ui;
};

#endif // FORMAUTHORMANAGE_H
