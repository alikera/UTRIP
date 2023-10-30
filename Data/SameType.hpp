#ifndef SAMETYPE
#define SAMETYPE
#include "Criteria.hpp"
class SameType
{
public:
	SameType(Criteria* criteria_);
	bool operator()(Criteria* c1);
private:
	Criteria* criteria;	
};
#endif