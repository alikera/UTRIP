#ifndef RATINGS_HPP
#define RATINGS_HPP

#include <list>
#include <string>
#include "Rating.hpp"
#include "GPA.hpp"

typedef std::list<Rating> ratings_t;

class Ratings
{
public:
	void set_rating(Rating user_rating);

	void show_GPA_of_ratings();
	bool user_has_submitted_rating(std::string username);
	double get_this_user_overall_rating(std::string username);
private:
	ratings_t all_ratings;

	GPA calculate_GPA_of_ratings();
	GPA calculate_sum_of_ratings();
	void devide_ratings_by_size(GPA& GPAs);
};

#endif