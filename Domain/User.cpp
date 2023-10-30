#include "User.hpp"
#include <iostream>
#include <sstream>
#include "defines.hpp"
#include "NotEnoughCredit.hpp"
#include "NotFound.hpp"
#include <typeinfo>
#include <algorithm>
#include <cmath>

using namespace std;
User::User(string email_, string username_, unsigned long int password_, int status_)
{
	email = email_;
	username = username_;
	password = password_;
	status = status_;
}

void User::logout()
{
	status = LOGGED_OUT;
	delete_criterias();
}

void User::login()
{
	status = LOGGED_IN;
}

void User::check_for_enough_money(double amount)
{
	if(my_wallet < -amount)
		throw NotEnoughCredit();
}

void User::charge_my_wallet(double amount)
{
	check_for_enough_money(amount);
	my_wallet += amount;
	financial_trans.push_front(my_wallet);
}

void User::show_my_recent_transactions(int count)
{
	if(count > financial_trans.size())
		count = financial_trans.size();
	
	auto it = financial_trans.begin();
	for(int i = 0; i < count; i++)
	{
		cout << int(round(*it)) << endl;
		++it;
	}
}

void User::set_user_reservation(Hotel* found_hotel, ReservationInputs reservation_info)
{
	MyReservation user_reservation = set_reservation_info(found_hotel, reservation_info);
	reservation_id_counter++;
	my_reservations.push_front(user_reservation);
}

MyReservation User::set_reservation_info(Hotel* found_hotel, ReservationInputs reservation_info)
{
	MyReservation user_reservation;
	user_reservation.reservation_info = reservation_info;
	int room_price = found_hotel->get_price_of_this_room_type(reservation_info.room_type);
	user_reservation.total_cost = calculate_total_cost(reservation_info, room_price);
	charge_my_wallet(-user_reservation.total_cost);
	user_reservation.reservation_id = reservation_id_counter;

	return user_reservation;
}

int User::calculate_total_cost(ReservationInputs reservation_info, int room_price)
{
	int stay_period = reservation_info.check_out - reservation_info.check_in + 1;
	return room_price * reservation_info.quantity * stay_period;
}

void User::set_user_rooms(rooms_t my_rooms)
{
	reserved_rooms.push_back(make_pair(reservation_id_counter-1, my_rooms));
}

void User::print_all_reservations()
{
	stringstream reservations_output;
	if(my_reservations.size() == 0)
	{
		cout << EMPTY_OUTPUT << endl;
		return;
	}

	for(auto const& it : my_reservations)
		print_each_reservations_info(it, reservations_output);

	cout << reservations_output.str();
}

void User::print_each_reservations_info(const MyReservation& it, 
	stringstream& reservations_output)
{
	reservations_output << ID_ << it.reservation_id
		<< HOTEL_ << it.reservation_info.hotel_id
		<< ROOM_ << it.reservation_info.room_type
		<< QUANTITY_ << it.reservation_info.quantity
		<< COST_ << it.total_cost
		<< _CHECK_IN_ << it.reservation_info.check_in
		<< _CHECK_OUT_ << it.reservation_info.check_out
		<< endl;
}

void User::cancel_reservation_by_id(int id)
{
	for(auto it = my_reservations.begin(); it != my_reservations.end(); ++it)
		if(it->reservation_id == id)
		{
			cancel_reservation(id, it->reservation_info);
			take_back_half_money(it->total_cost);
			it = my_reservations.erase(it);
			return;
		}

	throw NotFound();
}

void User::cancel_reservation(int id, ReservationInputs to_be_canceled)
{
	for(auto it = reserved_rooms.begin(); it != reserved_rooms.end(); ++it)
		if(it->first == id)
			free_rooms_period(it->second, to_be_canceled.check_in);
}

void User::free_rooms_period(rooms_t rooms, int check_in)
{
	for(auto const& it : rooms)
		it->free_reservation_period(check_in);
}

void User::take_back_half_money(int total_cost)
{
	charge_my_wallet(total_cost/2);
}

void User::set_criterias(Criteria* criteria)
{
	replace_if(active_criterias.begin(), active_criterias.end(), 
		 SameType(criteria), criteria);
	active_criterias.push_back(criteria);
}

hotels_t User::apply_criteria(hotels_t hotels)
{
	for(auto it : active_criterias)
		hotels = it->apply_criteria(hotels);

	return hotels;

}

void User::delete_criterias()
{
	active_criterias.erase(active_criterias.begin(), active_criterias.end());
}
