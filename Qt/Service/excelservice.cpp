#include "excelservice.h"
#include<QDir>
#include <windows.h>
#include <QDesktopServices>
#include <QUrl>

ExcelService::ExcelService()
{

}

///
/// \brief 数字转换为26字母
///
/// 1->A 26->Z
/// \param data
/// \return
///
QString ExcelService::to26AlphabetString(int data, int col)
{
    QChar ch = data + 0x40;//A对应0x41
    return QString(ch)+QString("%1").arg(col);
}

void ExcelService::addSheetRow(QAxObject *sheet, int row, QVector<QVariant> list)
{
    for(int i=0;i<list.size();++i){
        QString ch = to26AlphabetString(i+1,row);                               //生成列标如A1 B1
        QAxObject *cell = sheet->querySubObject("Range(QVariant, QVariant)",ch);//获取单元格
        cell->dynamicCall("SetValue(const QVariant&)",list.at(i));              //设置单元格的值
    }
}

void openExcel(QString &fileName)
{

    QFile file(fileName);
    if (file.exists()) {
        QDesktopServices::openUrl(QUrl::fromLocalFile(fileName));
    }
}


bool ExcelService::dataExport(QString filepath, QVector<QVariant> title, QVector<QVector<QVariant>> data, bool isOpen)
{
    HRESULT r = OleInitialize(0);
    if (r != S_OK && r != S_FALSE)
    {qWarning("Qt:初始化Ole失败（error %x）",(unsigned int)r);}

    QAxObject *excel = new QAxObject();
    excel->setControl("Excel.Application");                             //连接Excel控件
    excel->dynamicCall("SetVisible (bool Visible)","false");            //不显示窗体
    excel->setProperty("DisplayAlerts", false);                         //不显示任何警告信息。如果为true那么在关闭是会出现类似“文件已修改，是否保存”的提示

    QAxObject *workbooks = excel->querySubObject("WorkBooks");          //获取工作簿集合
    workbooks->dynamicCall("Add");                                      //新建一个工作簿
    QAxObject *workbook   = excel->querySubObject("ActiveWorkBook");    //获取当前工作簿
    QAxObject *worksheets = workbook->querySubObject("Sheets");         //获取工作表集合
    QAxObject *worksheet  = worksheets->querySubObject("Item(int)",1);  //获取工作表集合的工作表1，即sheet1

    ////添加表头
    addSheetRow(worksheet,1,title);

    for(int i =0;i<data.size();++i){
        addSheetRow(worksheet,i+2,data.at(i));
    }

    QString merge_cell = QString("A1:%1").arg(to26AlphabetString(title.size()+1,data.size()+1));      // 设置A1至Z10范围内的单元格的属性
    QAxObject *merge_range = worksheet->querySubObject("Range(const QString&)", merge_cell);
    merge_range->setProperty("HorizontalAlignment", -4108);         // 水平居中
    merge_range->setProperty("VerticalAlignment", -4108);           // 垂直居中
    merge_range->setProperty("NumberFormatLocal", "@");             // 设置为文本
    //  merge_range->setProperty("WrapText", true);                 //不够宽就换行
    merge_range->querySubObject("Font")->setProperty("Size", 12);   //设置字号
    delete merge_range;

    //  worksheet->querySubObject("Range(const QString&)", "1:1")->setProperty("RowHeight", 60);//调整第一行行高
    worksheet->querySubObject("Range(const QString&)", "1:1")->querySubObject("Font")->setProperty("Bold", true);
    worksheet->querySubObject("Range(const QString&)", "1:1")->querySubObject("Font")->setProperty("Size", 14);


    QAxObject * range = worksheet->querySubObject("UsedRange");
    if (range!=NULL)
    {
        QAxObject * cells = range->querySubObject("Columns");
        cells->dynamicCall("AutoFit");//自适应宽度
        delete range;
    }

    workbook->dynamicCall("SaveAs(const QString&)",QDir::toNativeSeparators(filepath));//保存至filepath，注意一定要用QDir::toNativeSeparators将路径中的"/"转换为"\"，不然一定保存不了。
    workbook->dynamicCall( "Close(Boolean)", true );//关闭工作簿
    excel->dynamicCall( "Quit(void)" );//关闭excel

    delete excel;
    excel=NULL;
    OleUninitialize();
    if(isOpen)
        openExcel(filepath);
    return true;
}

bool ExcelService::dataExportEx(QString filepath, QVector<QVariant> title,\
                                QVector<QVector<QVariant> > data, QVector<QVector<QVariant>> exData, bool isOpen)
{
    HRESULT r = OleInitialize(0);
    if (r != S_OK && r != S_FALSE)
    {qWarning("Qt:初始化Ole失败（error %x）",(unsigned int)r);}

    QAxObject *excel = new QAxObject();
    excel->setControl("Excel.Application");                             //连接Excel控件
    excel->dynamicCall("SetVisible (bool Visible)","false");            //不显示窗体
    excel->setProperty("DisplayAlerts", false);                         //不显示任何警告信息。如果为true那么在关闭是会出现类似“文件已修改，是否保存”的提示

    QAxObject *workbooks = excel->querySubObject("WorkBooks");          //获取工作簿集合
    workbooks->dynamicCall("Add");                                      //新建一个工作簿
    QAxObject *workbook   = excel->querySubObject("ActiveWorkBook");    //获取当前工作簿
    QAxObject *worksheets = workbook->querySubObject("Sheets");         //获取工作表集合
    QAxObject *worksheet  = worksheets->querySubObject("Item(int)",1);  //获取工作表集合的工作表1，即sheet1

    ////添加表头
    addSheetRow(worksheet,1,title);

    for(int i =0;i<data.size();++i){
        addSheetRow(worksheet,i+2,data.at(i));
    }



    QString merge_cell = QString("A1:%1").arg(to26AlphabetString(title.size()+1,data.size()+1));      // 设置A1至Z10范围内的单元格的属性
    QAxObject *merge_range = worksheet->querySubObject("Range(const QString&)", merge_cell);
    merge_range->setProperty("HorizontalAlignment", -4108);         // 水平居中
    merge_range->setProperty("VerticalAlignment", -4108);           // 垂直居中
    merge_range->setProperty("NumberFormatLocal", "@");             // 设置为文本
    //  merge_range->setProperty("WrapText", true);                 //不够宽就换行
    merge_range->querySubObject("Font")->setProperty("Size", 12);   //设置字号
    delete merge_range;


    for(int i =0;i<exData.size();++i){
        addSheetRow(worksheet,data.size()+4+i,exData.at(i));
    }
    QString merge_cell_Ex = QString("A%1:%2").arg(data.size()+5).arg(to26AlphabetString(title.size()+1,exData.size()+2+data.size()));      // 设置A1至Z10范围内的单元格的属性
    QAxObject *merge_range_Ex = worksheet->querySubObject("Range(const QString&)", merge_cell_Ex);
    merge_range_Ex->setProperty("HorizontalAlignment", -4108);         // 水平居中
    merge_range_Ex->setProperty("VerticalAlignment", -4108);           // 垂直居中
    merge_range_Ex->setProperty("NumberFormatLocal", "@");             // 设置为文本
    merge_range_Ex->querySubObject("Font")->setProperty("Size", 24);   //设置字号
    merge_range_Ex->querySubObject("Font")->setProperty("Bold", true);   //设置字号
    delete merge_range_Ex;


    //  worksheet->querySubObject("Range(const QString&)", "1:1")->setProperty("RowHeight", 60);//调整第一行行高
    worksheet->querySubObject("Range(const QString&)", "1:1")->querySubObject("Font")->setProperty("Bold", true);
    worksheet->querySubObject("Range(const QString&)", "1:1")->querySubObject("Font")->setProperty("Size", 14);


    QAxObject * range = worksheet->querySubObject("UsedRange");
    if (range!=NULL)
    {
        QAxObject * cells = range->querySubObject("Columns");
        cells->dynamicCall("AutoFit");//自适应宽度
        delete range;
    }

    workbook->dynamicCall("SaveAs(const QString&)",QDir::toNativeSeparators(filepath));//保存至filepath，注意一定要用QDir::toNativeSeparators将路径中的"/"转换为"\"，不然一定保存不了。
    workbook->dynamicCall( "Close(Boolean)", true );//关闭工作簿
    excel->dynamicCall( "Quit(void)" );//关闭excel

    delete excel;
    excel=NULL;
    OleUninitialize();
    if(isOpen)
        openExcel(filepath);
    return true;
}
