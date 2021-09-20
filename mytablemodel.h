#ifndef MYTABLEMODEL_H
#define MYTABLEMODEL_H

#include <QVariant>
#include <QAbstractTableModel>
#include "myrow.h"
#include <QSqlTableModel>

class MyTableModel :public QAbstractTableModel
{
public:
    void refreshData(QVector<myRow> table);
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;


private:
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;

    QVector<myRow> myTable;
};




#endif // MYTABLEMODEL_H
