#ifndef M_TABLEWIDGET_H
#define M_TABLEWIDGET_H
#include <QTableWidget>
#include <QHeaderView>
class M_TableWidget : public QTableWidget
{
    Q_OBJECT
public:
    M_TableWidget(QWidget *w=0);
    void removeAllRow();
    void setHeaderColModel( QHeaderView::ResizeMode mode);
public slots:
    void checkSelect();


};

#endif // M_TABLEWIDGET_H
