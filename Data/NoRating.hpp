#ifndef NORATING_HPP
#define NORATING_HPP
#include <exception>
#define NO_RATING "No Rating\n"

class NoRating : public std::exception
{
    const char* what() const throw()
    {
        return NO_RATING;
    }
};

#endif