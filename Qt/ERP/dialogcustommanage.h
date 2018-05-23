#ifndef DIALOGCUSTOMMANAGE_H
#define DIALOGCUSTOMMANAGE_H

#include <QDialog>

namespace Ui {
class DialogCustomManage;
}

class DialogCustomManage : public QDialog
{
    Q_OBJECT

public:
    explicit DialogCustomManage(QWidget *parent = 0);
    ~DialogCustomManage();

private:
    Ui::DialogCustomManage *ui;
};

#endif // DIALOGCUSTOMMANAGE_H
