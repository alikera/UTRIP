#include "User.hpp"
#include <iostream>
#include <sstream>
#include "NotEnoughCredit.hpp"
#include "NotFound.hpp"
#include "CriteriaPrice.hpp"
#include "CriteriaDefault.hpp"
#include "InsufficientRatings.hpp"
#include <typeinfo>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <random>

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
	default_filter = ON;
	manual_weights_condition = OFF;
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
	calculate_standard_deviation();
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
		{
			free_rooms_period(it->second, to_be_canceled.check_in);
			it = reserved_rooms.erase(it);
		}
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
	active_default_price_filter();
	for(auto it : active_criterias)
		hotels = it->apply_criteria(hotels);

	return hotels;
}

void User::delete_criterias()
{
	active_criterias.erase(active_criterias.begin(), active_criterias.end());
}

double User::calculate_standard_deviation()
{
	vector<int> rooms_price = list_reserved_rooms_prices();
	double average = calculate_average_price();
	double sigma = calculate_sigma_of_standard_deviation(rooms_price, average);

	double std_deviation = sqrt(sigma / (rooms_price.size() - 1));
	return std_deviation;
}

vector<int> User::list_reserved_rooms_prices()
{
	vector<int> prices;
	for(auto reservation = reserved_rooms.begin(); reservation != reserved_rooms.end(); ++reservation)
		for(auto room : reservation->second)
			prices.push_back(room->get_price());

	return prices;	
}

double User::calculate_average_price()
{
	vector<int> prices = list_reserved_rooms_prices();
	double average = 0;
	for (auto price : prices)
		average += price;

	return average / prices.size();
}

double User::calculate_sigma_of_standard_deviation(const vector<int>& prices,
	 const double& average)
{
	double sigma = 0;
	for(auto price : prices)
		sigma += pow(price - average, 2);

	return sigma;
}

void User::active_default_price_filter()
{
	if(default_filter_cant_be_activate())
	{
		delete_default_price_filter();
		return;
	}

	cout << RESAULTS_BY_DEFAULT_FILTER << endl;
	create_default_price_filter();
}

bool User::default_filter_cant_be_activate()
{
	if(reserved_rooms.size() < MIN_RESERVE_DEFAULT || default_filter == OFF)
		return true;

	for(auto criteria : active_criterias)
		if(price_filter_is_active(criteria))
			return true;

	return false;
}

bool User::price_filter_is_active(Criteria* this_criteria)
{
	return typeid(*this_criteria) == typeid(CriteriaPrice);
}

void User::delete_default_price_filter()
{
	for(auto it = active_criterias.begin(); it != active_criterias.end(); ++it)
		if(is_default_price_filter(*it))
			it = active_criterias.erase(it);
}

bool User::is_default_price_filter(Criteria* this_criteria)
{
	return typeid(*this_criteria) == typeid(CriteriaDefault);
}

void User::create_default_price_filter()
{
	double average = calculate_average_price();
	double deviation = calculate_standard_deviation();

	CriteriaDefault* criteria = new CriteriaDefault(average, deviation);
	set_criterias(criteria);
}

void User::set_default_filter_activation_status(const int& condition)
{
	default_filter = condition;
}

void User::set_manual_weights_condition(const int& condition)
{
	manual_weights_condition = condition;
}

void User::set_manual_weights(const map_t& weights)
{
	manual_weights = weights;
}

void User::print_manual_weights()
{
	cout << ACTIVE << SPACE << TRUEE << SPACE << fixed << setprecision(2)
	<< _LOCATION_ <<  truncate(manual_weights[_LOCATION_])
	<< _CLEANLINESS_ <<  truncate(manual_weights[_CLEANLINESS_])
	<< _STAFF_ <<  truncate(manual_weights[_STAFF_])
	<< _FACILITIES_ <<  truncate(manual_weights[_FACILITIES_])
	<< _VALUE_FOR_MONEY_ <<  truncate(manual_weights[_VALUE_FOR_MONEY_]) << SPACE << endl;
}

double User::truncate(const double& number)
{
	return floor(number * 100) / 100;
}

void User::add_rating(Rating user_rating)
{
	new_rating = YES;
	for(auto it = ratings.begin(); it != ratings.end(); ++it)
		if(it->id == user_rating.id)
		{
			*it = user_rating;
			return;
		}

	ratings.push_back(user_rating);
}

map_t User::get_estimated_weights()
{
	if(new_rating == YES)
		estimate_weights();

	return estimated_weights;
}

void User::estimate_weights()
{
    double num = throw_random_number_1_5_();
    declare_estimated_weights_map(num);
	map_t derivates = declare_map_of_derivates(num);

	for(int i = 0; i < 2000; i++)
	{
		reset_derivates(derivates);
		calculate_derivates(derivates);
		update_estimate_weights(derivates);
		check_for_out_of_range();
	}

	new_rating = NO;
}

int User::throw_random_number_1_5_()
{
	srand(time(0));
    int num = (rand() % 5) + 1;

    return num;
}

void User::declare_estimated_weights_map(const int& num)
{
	estimated_weights[_LOCATION_] = num;
	estimated_weights[_FACILITIES_] = num;
	estimated_weights[_STAFF_] = num;
	estimated_weights[_CLEANLINESS_] = num;
	estimated_weights[_VALUE_FOR_MONEY_] = num;
}

map_t User::declare_map_of_derivates(const int& num)
{
	map_t derivates;

	derivates[_LOCATION_] = num;
	derivates[_FACILITIES_] = num;
	derivates[_STAFF_] = num;
	derivates[_CLEANLINESS_] = num;
	derivates[_VALUE_FOR_MONEY_] = num;

	return derivates;
}

void User::reset_derivates(map_t& derivates)
{
	for_each(derivates.begin(), derivates.end(), [](pair<const string, double>& d)
	{
		d.second = 0;
	});
}

void User::calculate_derivates(map_t& derivates)
{
	for(auto rating : ratings)
		for_each(derivates.begin(), derivates.end(), [rating, this](pair<const string, double>& d)
		{
			d.second += derivate_error_function(rating, d.first);
		});
}

void User::update_estimate_weights(const map_t& derivates)
{
	for(auto weight = estimated_weights.begin(); weight != estimated_weights.end(); ++weight)
		for(auto derivate : derivates)
			if(weight->first == derivate.first)
				weight->second = weight->second - derivate.second;
}

void User::check_for_out_of_range()
{
	for_each(estimated_weights.begin(), estimated_weights.end(),[](pair<const string, double>& weight)
	{
		if(weight.second > 5)
			weight.second = 5;

		if(weight.second < 1)
			weight.second = 1;
	});
}

double User::derivate_error_function(Rating rating, const string& key)
{
	map_t new_weights = increase_estimated_weights_by_epsilon(key);
	double soorat = error_function(rating, new_weights) - error_function(rating, estimated_weights);
	return  soorat / EPSILON;
}

map_t User::increase_estimated_weights_by_epsilon(const string& key)
{
	map_t new_weights = estimated_weights;
	new_weights[key] = new_weights[key] + EPSILON;

	return new_weights;
}

double User::error_function(Rating rating, map_t weights)
{
	return pow(estimate_rating(rating, weights) - rating.overall_rating, 2);
}

double User::estimate_rating(Rating rating, map_t weights)
{
	double sum = calculate_sum_weights_multiply_ratings(rating, weights);
	double sum_weights = calculate_sum_of_weights(weights);

	return sum / sum_weights;
}

double User::calculate_sum_weights_multiply_ratings(const Rating& rating, map_t weights)
{
	double sum = 0;
	sum += weights[_LOCATION_] * rating.location;
	sum += weights[_FACILITIES_] * rating.facilities;
	sum += weights[_CLEANLINESS_] * rating.cleanliness;
	sum += weights[_STAFF_] * rating.staff;
	sum += weights[_VALUE_FOR_MONEY_] * rating.value_for_money;

	return sum;	
}

double User::calculate_sum_of_weights(const map_t& weights)
{
	double sum_weights = 0;

	for(auto w : weights)
		sum_weights += w.second;

	return sum_weights;
}

void User::print_estimated_weights()
{
	check_for_sufficient_ratings();
	if(new_rating == YES)
		estimate_weights();

	cout << fixed << setprecision(2)
	<< LOCATION_ << truncate(estimated_weights[_LOCATION_])
	<< _CLEANLINESS_ << truncate(estimated_weights[_CLEANLINESS_])
	<< _STAFF_ << truncate(estimated_weights[_STAFF_])
	<< _FACILITIES_ << truncate(estimated_weights[_FACILITIES_])
	<< _VALUE_FOR_MONEY_ << truncate(estimated_weights[_VALUE_FOR_MONEY_]) << SPACE << endl;
}

void User::check_for_sufficient_ratings()
{
	if(ratings.size() < 5)
		throw InsufficientRatings();
}

