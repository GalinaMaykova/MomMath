#include "databaseexception.h"

DataBaseException::DataBaseException(QString ex)
{
    m_ex = ex;
}

QString DataBaseException::getMsg()
{
    return m_ex;
}
