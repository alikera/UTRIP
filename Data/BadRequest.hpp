#ifndef BADREQUEST_HPP
#define BADREQUEST_HPP
#include <exception>
#define BAD_REQUEST "Bad Request\n"

class BadRequest : public std::exception
{
    const char* what() const throw()
    {
        return BAD_REQUEST;
    }
};

#endif