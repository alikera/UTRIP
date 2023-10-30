#ifndef CrITERIASTAR_HPP
#define CrITERIASTAR_HPP

#include "Criteria.hpp"
#include <string>

class CriteriaStar : public Criteria
{
public:
	CriteriaStar(int min_star_, int max_star_);
	hotels_t apply_criteria(hotels_t hotels);
private:
	int min_star;
	int max_star;
};

#endif