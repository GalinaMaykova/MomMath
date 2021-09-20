#ifndef MYSQLSEARCHER_H
#define MYSQLSEARCHER_H

#include "myrow.h"
#include <QVector>
#include <QtSql>

class mySQLsearcher
{
public:
    static mySQLsearcher* GetInstance();
    QVector<myRow> GetTableAllTask();
    void SetDBPath(QString path);
    QList<QPair<int , QString>> LoadProperties();
    QVector<myRow> GetTableTaskWithPtoperties(QVector<int> PropertiesID);
    void GetTaskPNG(QString taskId, QByteArray& imgFromDB);
private:
    mySQLsearcher();
    static mySQLsearcher* m_SQLsearcher;
    QSqlDatabase sdb;
};

#endif // MYSQLSEARCHER_H
