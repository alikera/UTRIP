#ifndef INSUFFICIENTRATINGS_HPP
#define INSUFFICIENTRATINGS_HPP

#include <exception>
#define INSUFFICIENT_RATINGS "Insufficient Ratings\n"

class InsufficientRatings : public std::exception
{
    const char* what() const throw()
    {
        return INSUFFICIENT_RATINGS;
    }
};

#endif