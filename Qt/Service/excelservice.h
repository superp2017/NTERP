#ifndef EXCELSERVICE_H
#define EXCELSERVICE_H
#include <QString>
#include <QAxObject>
#include <QVector>

#pragma execution_character_set("utf-8")
class ExcelService
{
public:
    ExcelService();
   static QString to26AlphabetString(int data,int col);
   static void addSheetRow(QAxObject *sheet, int row, QVector<QVariant> list);
   static bool dataExport(QString filepath, QVector<QVariant>title, QVector<QVector<QVariant>> data,bool isOpen=false);
   static bool dataExportEx(QString filepath, QVector<QVariant>title, QVector<QVector<QVariant>> data, QVector<QVector<QVariant> > exData, bool isOpen=false);

};

#endif // EXCELSERVICE_H
