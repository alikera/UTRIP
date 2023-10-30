#ifndef ROOMSDETAIL_HPP
#define ROOMSDETAIL_HPP

struct RoomsDetail
{
	int standard_price;
	int delux_price;
	int luxury_price;
	int premium_price;

	double average_price;
};

typedef struct RoomsDetail RoomsDetail;
#endif