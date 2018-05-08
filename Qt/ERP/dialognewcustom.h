#ifndef DIALOGNEWCUSTOM_H
#define DIALOGNEWCUSTOM_H

#include <QDialog>

namespace Ui {
class DialogNewCustom;
}

class DialogNewCustom : public QDialog
{
    Q_OBJECT

public:
    explicit DialogNewCustom(QWidget *parent = 0);
    ~DialogNewCustom();

private:
    Ui::DialogNewCustom *ui;
};

#endif // DIALOGNEWCUSTOM_H
