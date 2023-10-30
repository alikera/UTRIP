#include "SameType.hpp"

using namespace std;
SameType::SameType(Criteria* criteria_)
{
	criteria = criteria_;
}

bool SameType::operator()(Criteria* c1)
{
	return typeid(*c1) == typeid(*criteria);
}