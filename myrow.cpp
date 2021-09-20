#include "myrow.h"

    myRow::myRow(int id, QString task):m_id(id), m_task(task){}

    int myRow::getID() const
    {
        return m_id;
    }

    QString myRow::getTask() const
    {
        return m_task;
    }


