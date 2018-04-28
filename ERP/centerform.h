#ifndef CENTERFORM_H
#define CENTERFORM_H

#include <QWidget>
#include <QStackedLayout>
#include "mainspace.h"
#include "login.h"

namespace Ui {
class CenterForm;
}

class CenterForm : public QWidget
{
    Q_OBJECT

public:
    explicit CenterForm(QWidget *parent = 0);
    ~CenterForm();
public slots:
    void test();

private:
    QStackedLayout m_layout;
    Login          m_login;
    MainSpace      m_space;
};

#endif // CENTERFORM_H
