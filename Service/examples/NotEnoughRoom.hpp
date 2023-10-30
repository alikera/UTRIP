#ifndef NOTENOUGHROOM_HPP
#define NOTENOUGHROOM_HPP
#include <exception>
#define NOT_ENOUGH_ROOM "Not Enough Room\n"

class NotEnoughRoom : public std::exception
{
    const char* what() const throw()
    {
        return NOT_ENOUGH_ROOM;
    }
};

#endif