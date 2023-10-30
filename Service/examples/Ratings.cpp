#include "Ratings.hpp"
#include "defines.hpp"
#include "NoRating.hpp"
using namespace std;
#include <iostream>
#include <iomanip>

void Ratings::set_rating(Rating user_rating)
{
	for(auto it = all_ratings.begin(); it != all_ratings.end(); ++it)
		if(it->id == user_rating.id)
		{
			*it = user_rating;
			return;
		}
		
	all_ratings.push_back(user_rating);
}

void Ratings::show_GPA_of_ratings()
{
	GPA GPAs = calculate_GPA_of_ratings();

	cout << LOCATION_ << fixed << setprecision(2) << GPAs.location << 
	endl << CLEANLINESS_ << fixed << setprecision(2) << GPAs.cleanliness <<
	endl << STAFF_ << fixed << setprecision(2) << GPAs.staff <<
	endl << FACILITIES_ << fixed << setprecision(2) << GPAs.facilities <<
	endl << VALUE_FOR_MONEY_ << fixed << setprecision(2) << GPAs.value_for_money <<
	endl << OVERALL_RATING_ << fixed << setprecision(2) << GPAs.overall_rating << endl;
}

GPA Ratings::calculate_GPA_of_ratings()
{
	if(all_ratings.size() == 0)
		throw NoRating();
	GPA GPAs = calculate_sum_of_ratings();
	devide_ratings_by_size(GPAs);

	return GPAs;
}

GPA Ratings::calculate_sum_of_ratings()
{
	GPA GPAs;
	for(auto it = all_ratings.begin(); it != all_ratings.end(); ++it)
	{
		GPAs.location += it->location;
		GPAs.cleanliness += it->cleanliness;
		GPAs.staff += it->staff;
		GPAs.facilities += it->facilities;
		GPAs.value_for_money += it->value_for_money;
		GPAs.overall_rating += it->overall_rating;
	}
	return GPAs;
}

void Ratings::devide_ratings_by_size(GPA& GPAs)
{
	GPAs.location /= all_ratings.size();
	GPAs.cleanliness /= all_ratings.size();
	GPAs.staff /= all_ratings.size();
	GPAs.facilities /= all_ratings.size();
	GPAs.value_for_money /= all_ratings.size();
	GPAs.overall_rating /= all_ratings.size();
}

bool Ratings::user_has_submitted_rating(string username)
{
	for(auto it : all_ratings)
		if(it.id == username)
			return true;

	return false;
}

double Ratings::get_this_user_overall_rating(string username)
{
	for(const auto& it : all_ratings)
		if(it.id == username)
			return it.overall_rating;
}
