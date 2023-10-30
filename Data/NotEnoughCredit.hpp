#ifndef NOTENOUGHCREDIT_HPP
#define NOTENOUGHCREDIT_HPP
#include <exception>
#define NOT_ENOUGH_CREDIT "Not Enough Credit\n"

class NotEnoughCredit : public std::exception
{
    const char* what() const throw()
    {
        return NOT_ENOUGH_CREDIT;
    }
};

#endif