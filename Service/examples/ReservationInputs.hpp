#ifndef RESERVATIONINPUTS_HPP
#define RESERVATIONINPUTS_HPP

struct ReservationInputs
{
	std::string hotel_id;
	std::string room_type;
	int quantity;
	int check_in;
	int check_out;
};
typedef struct ReservationInputs ReservationInputs;

#endif