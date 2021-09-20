#include "mysqlsearcher.h"
#include <QtSql>
#include "databaseexception.h"

mySQLsearcher* mySQLsearcher::m_SQLsearcher = nullptr;

mySQLsearcher::mySQLsearcher()
{}


mySQLsearcher* mySQLsearcher::GetInstance()
{
    if(m_SQLsearcher==nullptr){
            m_SQLsearcher = new mySQLsearcher();
        }
        return m_SQLsearcher;
}


void mySQLsearcher::SetDBPath(QString path)
{
    sdb = QSqlDatabase::addDatabase("QSQLITE");
    sdb.setDatabaseName(path);
}


QVector<myRow> mySQLsearcher::GetTableAllTask()
{
    QVector<myRow> vec;
    QString myQuery = "select TaskId, headline from task";
    if (sdb.open())
    {
        QSqlQuery query;
        if (!query.exec(myQuery))
        {
            sdb.close();
            throw DataBaseException("Запрос не удался");
        }
        if (!query.first())
        {
            sdb.close();
            throw DataBaseException("Нет задач в базе");
        }
        // простой запрос
        do
        {
            vec.push_back(myRow(query.value(0).toInt(), query.value(1).toString()));
        } while(query.next());
        sdb.close();
    }
    else
    {
        throw DataBaseException("База не открылась");
    }
    return vec;
}


QList<QPair<int , QString>> mySQLsearcher::LoadProperties()
{
    QList<QPair<int,QString>> answer;
    if (sdb.open())
    {
        QSqlQuery query;
        if (!query.exec("select PropertyID, propertyNAME from properties"))
        {
            sdb.close();
            throw DataBaseException("Запрос критериев не удался");
        }
        if (!query.first())
        {
             sdb.close();
            throw DataBaseException("Нет критериев в базе");
        }
        do
        {
            int propID = query.value(0).toInt();
            QString propertyName =  query.value(1).toString();
            answer.append(qMakePair(propID, propertyName));
        } while(query.next());
        sdb.close();
    }
    else
    {
        throw DataBaseException("База не открылась");
    }
    return answer;
}

// Вывод в таблицу задач, которые соответствуют списку критериев
// Если вектор пустой, выводятся все задачи
// PropertiesID - критерии, по которым искать.
QVector<myRow> mySQLsearcher::GetTableTaskWithPtoperties(QVector<int> PropertiesID)
{
    if(PropertiesID.size() == 0)
    {
        return GetTableAllTask();
    }
    QVector<myRow> vec;
    QString temp;
    for(int i=0; i<PropertiesID.size() - 1; ++i)
    {
        temp += QString::number(PropertiesID[i]);
        temp += ", ";
    }
    temp +=QString::number(PropertiesID[PropertiesID.size() - 1]);

    QString myQuery = "select TaskId from reference "
            "WHERE PropertyID IN ("+ temp + ") "
            "GROUP BY TaskID "
            "HAVING count(distinct PropertyID) = " + QString::number(PropertiesID.size());
    if (sdb.open())
    {
        QSqlQuery query;
        if (!query.exec(myQuery)){
            sdb.close();
            throw DataBaseException("Запрос не удался");
        }
        if (!query.first()) {
            sdb.close();
            throw DataBaseException("Нет задач в базе");
        }
        do {
            // из запроса query получаем ID подходящих задач
           int taskID = query.value(0).toInt();
           //создаем запрос, чтоб по ID получить всю инфу о задаче
            QSqlQuery query1;
           if (!query1.exec("select TaskId, headline from task where TaskId = " + QString::number(taskID))){
               sdb.close();
               throw DataBaseException("Запрос не удался");
           }
           query1.first();
           vec.push_back(myRow(query1.value(0).toInt(), query1.value(1).toString()));
       }while(query.next());
    }
    else
    {
        throw DataBaseException("База не открылась");
    }
    return vec;
}

void mySQLsearcher::GetTaskPNG(QString taskId, QByteArray& imgFromDB)
{
    if (sdb.open())
    {
        QSqlQuery query;
        if (!query.exec("select text from task where TaskId = " + taskId))
        {
            sdb.close();
            throw DataBaseException("Невозможно загрузить картинку");
            return;
        }
        query.first();
        imgFromDB = query.value(0).toByteArray();
        sdb.close();
    }
    else
    {
        throw DataBaseException("База не открылась");
    }
    return;
}
