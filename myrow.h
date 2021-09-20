#ifndef MYROW_H
#define MYROW_H

#include <QString>

class myRow
{
public:
    myRow(int id, QString task);
    int getID() const;
    QString getTask() const;
    static const int m_count = 2;
private:
    int m_id;
    QString m_task;
};

#endif // MYROW_H
