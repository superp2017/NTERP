#ifndef DIALOGVERSIONUPDATE_H
#define DIALOGVERSIONUPDATE_H

#include <QDialog>
#include "service_global.h"
#pragma execution_character_set("utf-8")
namespace Ui {
class DialogVersionUpdate;
}

class DialogVersionUpdate : public QDialog
{
    Q_OBJECT

public:
    explicit DialogVersionUpdate(QWidget *parent = 0);
    ~DialogVersionUpdate();
    void setVersion(VersionInfo &info);
private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_clicked();
private:
    Ui::DialogVersionUpdate *ui;
    VersionInfo m_net_version;
    QString m_url;
};

#endif // DIALOGVERSIONUPDATE_H
