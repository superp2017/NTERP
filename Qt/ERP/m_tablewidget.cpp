#include "m_tablewidget.h"

M_TableWidget::M_TableWidget(QWidget *w):QTableWidget(w)
{

}

void M_TableWidget::removeAllRow()
{
    int count = this->rowCount();
    for(int i=0;i<count;++i){
        this->removeRow(0);
    }
}

void M_TableWidget::setHeaderColModel(QHeaderView::ResizeMode mode)
{
    this->horizontalHeader()->setSectionResizeMode(mode);
}
