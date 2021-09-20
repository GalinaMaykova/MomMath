#ifndef BASEEXCEPTION_H
#define BASEEXCEPTION_H
#include <exception>
#include <QString>
class DataBaseException : public std::exception
{
public:
    DataBaseException(QString ex);
    QString getMsg() ;
private:
    QString m_ex;

};

#endif // BASEEXCEPTION_H
