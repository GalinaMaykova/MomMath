#ifndef BASEEXCEPTION_H
#define BASEEXCEPTION_H
#include <exception>

class DataBaseException : public std::exception
{
public:
    DataBaseException();
};

#endif // BASEEXCEPTION_H
