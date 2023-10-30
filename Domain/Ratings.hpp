#ifndef RATINGS_HPP
#define RATINGS_HPP

#include <list>
#include "Rating.hpp"
#include "GPA.hpp"

typedef std::list<Rating> ratings_t;

class Ratings
{
public:
	void set_rating(Rating user_rating);

	void show_GPA_of_ratings();
private:
	ratings_t all_ratings;

	GPA calculate_GPA_of_ratings();
	GPA calculate_sum_of_ratings();
	void devide_ratings_by_size(GPA& GPAs);
};

#endif