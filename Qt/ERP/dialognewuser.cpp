#include "dialognewuser.h"
#include "ui_dialognewuser.h"
#include <QCompleter>
#include <QToolTip>
#include <QToolTip>
#include "datacenter.h"
#include "boost/thread.hpp"
#include <QRegExp>
#include <QValidator>

DialogNewUser::DialogNewUser(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogNewUser),
    m_isNewMode(true)
{
    ui->setupUi(this);

    ui->dateEdit_in_time->setDate(QDate::currentDate());
    ui->dateEdit_in_time->setMinimumDate(QDate::currentDate().addDays(-365*10));
    ui->dateEdit_in_time->setMaximumDate(QDate::currentDate().addDays(365*10));
    ui->dateEdit_in_time->setDisplayFormat("yyyy-MM-dd");
    ui->dateEdit_in_time->setCalendarPopup(true);

    QRegExp regx("[a-zA-Z0-9]+$");
    QValidator *validator = new QRegExpValidator(regx, this );
    ui->lineEdit_accout->setValidator( validator );
    ui->lineEdit_code->setValidator(validator);

    QRegExp rx("[0-9]+$");
    QRegExpValidator *vali = new QRegExpValidator(rx, this);
    ui->lineEdit_cell->setValidator(vali);



    connect(dataCenter::instance(),SIGNAL(sig_newEmployee(User,bool)),this,SLOT(creatUserCb(User,bool)));
    connect(dataCenter::instance(),SIGNAL(sig_modEmployee(User,bool)),this,SLOT(modUserCb(User,bool)));
}

DialogNewUser::~DialogNewUser()
{
    delete ui;
}


void DialogNewUser::changeModel()
{
    if(m_isNewMode){
        this->setWindowTitle("新增员工");
        ui->pushButton_creat->setText("创建");
    }else{
        this->setWindowTitle("员工修改");
        ui->pushButton_creat->setText("修改");
    }
}

User DialogNewUser::CurUser() const
{
    return m_curUser;
}

void DialogNewUser::initUI(User user)
{
    ui->lineEdit_name->setText(user.Name);
    ui->lineEdit_cell->setText(user.Cell);
    ui->comboBox_department->setCurrentText(user.Department);
    ui->comboBox_sex->setCurrentText(user.Sex);
    ui->spinBox_age->setValue(user.Age);
    ui->spinBox_salary->setValue(user.Salary);
    ui->dateEdit_in_time->setDate(QDate::fromString(user.InTime,"yyyy-MM-dd"));
    ui->lineEdit_accout->setText(user.Account);
    ui->lineEdit_code->setText(user.Code);
    ui->comboBox_author->setCurrentIndex(user.Author);
    m_curUser = user;
}

void DialogNewUser::clearUI()
{
    ui->lineEdit_name->setText("");
    ui->lineEdit_cell->setText("");
    ui->comboBox_department->setCurrentIndex(0);
    ui->comboBox_sex->setCurrentIndex(0);
    ui->spinBox_age->setValue(18);
    ui->spinBox_salary->setValue(0);
    ui->dateEdit_in_time->setDate(QDate::currentDate());
    ui->lineEdit_accout->setText("");
    ui->lineEdit_code->setText("");
}


void DialogNewUser::setModel(bool isNew)
{
    m_isNewMode = isNew;
    if(m_isNewMode){
        ui->pushButton_creat->setText("新增");
        this->setWindowTitle("新增员工");
    }else{
        this->setWindowTitle("员工修改");
        ui->pushButton_creat->setText("修改");
    }
    initComBox(dataCenter::instance()->pub_getDepartments(),dataCenter::instance()->pub_getAuthors());

}

void DialogNewUser::on_pushButton_creat_clicked()
{

    User user = m_curUser;
    user.Name        = ui->lineEdit_name->text().trimmed();
    user.Cell        = ui->lineEdit_cell->text().trimmed();
    user.Department  = ui->comboBox_department->currentText().trimmed();
    user.Author      = ui->comboBox_author->currentData().toInt();
    user.Sex         = ui->comboBox_sex->currentText().trimmed();
    user.InTime      = ui->dateEdit_in_time->text().trimmed();
    user.Salary      = ui->spinBox_salary->value();
    user.Age         = ui->spinBox_age->value();
    user.Account     = ui->lineEdit_accout->text().trimmed();
    user.Code        = ui->lineEdit_code->text().trimmed();

    if(user.Name.isEmpty()){
        QToolTip::showText(ui->lineEdit_name->mapToGlobal(QPoint(100, 0)), "姓名不能为空!");
        return ;
    }
    if(user.Cell.isEmpty()||user.Cell.length()<11){
        QToolTip::showText(ui->lineEdit_cell->mapToGlobal(QPoint(100, 0)), "手机号填写不正确!");
        return ;
    }

    if(user.Account.trimmed().isEmpty()){
        QToolTip::showText(ui->lineEdit_accout->mapToGlobal(QPoint(100, 0)), "账号不能为空!");
        return ;
    }

    if(!dataCenter::instance()->pub_checkDepartment(user.Department)){
        QToolTip::showText(ui->comboBox_department->mapToGlobal(QPoint(100, 0)), "部门不存在!");
        return ;
    }

    if(user.Code.trimmed().isEmpty()){
        QToolTip::showText(ui->lineEdit_code->mapToGlobal(QPoint(100, 0)), "密码不能为空!");
        return ;
    }

    if(user.Account=="Admin"){
        QToolTip::showText(ui->lineEdit_accout->mapToGlobal(QPoint(100, 0)), "不能创建Admin账户!");
        return ;
    }


    if(m_curUser.Author==4){
        AUTHOR_Limit(4);
    }
    if(user.Author==4){
        AUTHOR_Limit(4);
    }


    QJsonObject para = UserService::toJsonObject(user);
    if(m_isNewMode){
        boost::thread t(boost::bind(&dataCenter::net_newUser,dataCenter::instance(),para));
        t.detach();
    }else{
        boost::thread t(boost::bind(&dataCenter::net_modUser,dataCenter::instance(),para));
        t.detach();
    }
    dataCenter::instance()->pub_showLoadding("正在网络请求...",5000,Qt::black);
}

void DialogNewUser::on_pushButton_cancle_clicked()
{
    done(-1);
}

void DialogNewUser::creatUserCb(User user, bool ok)
{
    dataCenter::instance()->pub_hideLoadding();
    if(ok){
        dataCenter::instance()->pub_showMessage("创建成功!",4000);
        m_curUser = user;
        done(123);
    }else{
        dataCenter::instance()->pub_showMessage("创建失败!",4000);
        m_curUser = user;
        done(0);
    }
}

void DialogNewUser::modUserCb(User user, bool ok)
{
    dataCenter::instance()->pub_hideLoadding();
    if(ok){
        dataCenter::instance()->pub_showMessage("修改成功!",4000);
        m_curUser = user;
        done(123);
    }else{
        dataCenter::instance()->pub_showMessage("修改失败!",4000);
        m_curUser = user;
        done(0);
    }
}

void DialogNewUser::initComBox(QVector<QString>department,QVector<QString>autor)
{
    ui->comboBox_department->blockSignals(true);
    ui->comboBox_author->blockSignals(true);
    ui->comboBox_sex->addItem("男");
    ui->comboBox_sex->addItem("女");

    ui->comboBox_department->clear();
    //    QCompleter *completerauthor = new QCompleter(department.toList(), this);
    //    ui->comboBox_department->setEditable(true);
    //    for(QString de:department){
    //        ui->comboBox_department->addItem(de);
    //    }
    //    ui->comboBox_department->setCompleter(completerauthor);
    ui->comboBox_department->addItems(department.toList());

    ui->comboBox_author->clear();
    for(int i=0;i<autor.size();++i){
        if(i==4&&m_isNewMode){
            if(dataCenter::instance()->pub_CurUser().Author<4){
                continue;
            }
        }
        ui->comboBox_author->addItem(autor.at(i),i);
    }

    //     ui->comboBox_author->addItem(ItemNewAuthor);

    ui->comboBox_department->blockSignals(false);
    ui->comboBox_author->blockSignals(false);
}
