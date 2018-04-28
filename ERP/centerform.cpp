#include "centerform.h"
#include <QPushButton>
CenterForm::CenterForm(QWidget *parent) :
    QWidget(parent)
{
    m_layout.addWidget(&m_login);
    m_layout.addWidget(&m_space);
    this->setLayout(&m_layout);
        QPushButton * aaa = new QPushButton("login",this);
        connect(aaa,SIGNAL(clicked(bool)),this,SLOT(test()));
}

CenterForm::~CenterForm()
{

}

void CenterForm::test()
{
    m_layout.setCurrentIndex(1);
}
