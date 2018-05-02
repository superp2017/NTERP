#ifndef CENTERFORM_H
#define CENTERFORM_H

#include <QWidget>
#include <QStackedLayout>
#pragma execution_character_set("utf-8")
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


private:

};

#endif // CENTERFORM_H
