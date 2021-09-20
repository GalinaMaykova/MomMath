#include "mytablemodel.h"



void MyTableModel::refreshData(QVector<myRow> table)
{
    myTable = table;
}


int MyTableModel::rowCount(const QModelIndex &parent) const
{
    return myTable.size();
}

int MyTableModel::columnCount(const QModelIndex &parent) const
{
    return myRow::m_count;
}

QVariant MyTableModel::data(const QModelIndex &index, int role) const
{
    QString result;
    if (role == Qt::DisplayRole) {
        if(myTable.size()>index.row())
        {
            if(index.column()==0)
            {
                 result = QString::number(myTable[index.row()].getID());
            }
            else
            {
                result = myTable[index.row()].getTask();
            }
        }
    }
    return QVariant(result);
}


QVariant MyTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();
    if (orientation == Qt::Horizontal)
    {
        switch (section)
        {
        case 0:
            return QVariant(QString::fromUtf8("ID задачи"));
        case 1:
            return QVariant(QString::fromUtf8("Подходящие задачи:"));
        default:
            return QVariant();
        }
    }
    return QVariant();
}
