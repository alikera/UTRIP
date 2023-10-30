#ifndef RATING_HPP
#define RATING_HPP
#include <string>

struct Rating
{
	std::string username;

	double location;
	double cleanliness;
	double staff;
	double facilities;
	double value_for_money;
	double overall_rating;
};

typedef struct Rating Rating;

#endif