#ifndef NOTFOUND_HPP
#define NOTFOUND_HPP
#include <exception>
#define NOT_FOUND "Not Found\n"

class NotFound : public std::exception
{
    const char* what() const throw()
    {
        return NOT_FOUND;
    }
};

#endif