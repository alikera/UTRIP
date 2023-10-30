#include "MatchingId.hpp"
using namespace std;

MatchingId::MatchingId(string id_)
{
	id = id_;
}

bool MatchingId::operator()(Hotel* hotel)
{
	return id == hotel->get_uid();
}