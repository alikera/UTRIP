#include "Utrip.hpp"
#include "BadRequest.hpp"
#include "NotFound.hpp"
#include "functions.hpp"
#include "MatchingId.hpp"
#include "PermissionDenied.hpp"
#include "CriteriaCity.hpp"
#include "CriteriaStar.hpp"
#include "CriteriaPrice.hpp"
#include "CriteriaAvailableRooms.hpp"

#include <iostream>
#include <sstream>
#include <fstream>
#include <iterator>
#include <algorithm>

using namespace std;

Utrip::~Utrip()
{
	for(auto hotel : hotels)
		delete hotel;
	for(auto user : users)
		delete user;
}

void Utrip::read_from_file(string directory)
{
	ifstream hotels_file(directory);
	string line, item;
	getline(hotels_file, line);
	for(int j = 0; getline(hotels_file, line); j++)
	{
		Hotel* each_hotel = new Hotel;
		each_hotel->create(line);
		hotels.push_back(each_hotel);
	}
	sort(hotels.begin(), hotels.end(), compare_uid);
}

void Utrip::get_requests()
{
	string inputs;
	while(getline(cin >> ws, inputs))
	{
		try
		{
			process_request(inputs);
		}catch(exception& ex)
		{
			cout << ex.what();
		}
	}
}

void Utrip::process_request(string inputs)
{
	stringstream ss_inputs(inputs);
	analyze_first_command(ss_inputs);
}

string Utrip::get_command(stringstream& ss_inputs)
{
	string command;
	getline(ss_inputs, command, SPACE);
	return command;
}

void Utrip::analyze_first_command(stringstream& ss_inputs)
{
	string command = disassemble_input(ss_inputs);
	if(command == POST)
		process_command_after_POST(ss_inputs);

	else if(command == GET)
		process_command_after_GET(ss_inputs);

	else if(command == DELETE)
		process_command_after_DELETE(ss_inputs);

	else
		throw BadRequest();
}

void Utrip::process_command_after_POST(stringstream& ss_inputs)
{
	string command = disassemble_input(ss_inputs);
	auto logged_user = find_if(users.begin(), users.end(), logged_in);	

	if(logged_user == users.end())
		process_signup_or_login_request(ss_inputs, command);
	else
		process_logged_user_requests_POST(ss_inputs, command, *logged_user);

}

void Utrip::process_signup_or_login_request(stringstream& ss_inputs,
	 string command)
{
	if(command == SIGNUP)
	{
		check_for_qmark(ss_inputs);
		process_signup(ss_inputs);
	}
	else if(command == LOGIN)
	{
		check_for_qmark(ss_inputs);
		process_login(ss_inputs);
	}
	else
		throw PermissionDenied();
}

void Utrip::process_logged_user_requests_POST(stringstream& ss_inputs, 
	 string command, User* logged_user)
{
	if(command == LOGOUT)
		process_logout(logged_user);

	else if(command == SIGNUP)
		throw BadRequest();
	else if(command == LOGIN)
		throw BadRequest();

	else
		process_user_practical_requests(ss_inputs, command, logged_user);
}

void Utrip::process_logout(User* logged_user)
{
	logged_user->logout();
	cout << OK << endl;
}

void Utrip::process_user_practical_requests(stringstream& ss_inputs, 
	 string command, User* logged_user)
{
	check_for_qmark(ss_inputs);
	if(command == WALLET)
		process_wallet_charging(ss_inputs, logged_user);
	
	else if(command == RESERVES)
		process_reservation(ss_inputs, logged_user);
	
	else if(command == COMMENTS)
		process_posting_comment(ss_inputs, logged_user);

	else if(command == RATINGS)
		process_rating(ss_inputs, logged_user);

	else if(command == FILTERS)
		process_filtering(ss_inputs, logged_user);

	else
		throw NotFound();
}

void Utrip::process_signup(stringstream& ss_inputs)
{
	string email, username, password;
	for(int i = 0; i < 3; i++)
		get_user_information(email, username, password, ss_inputs);

	unsigned long int hashed_password = generate_password_hash(password);
	User* new_user = new User(email, username, hashed_password, LOGGED_IN);
	users.push_back(new_user);
	cout << OK << endl;
}

void Utrip::get_user_information(string& email, string& username, string& password, 
	stringstream& ss_inputs)
{
	string command = disassemble_input(ss_inputs);
	if(command == EMAIL && email == EMPTY)
		email = get_email_address(ss_inputs);

	else if(command == USERNAME && username == EMPTY)
		username = get_username(ss_inputs);

	else if(command == PASSWORD && password == EMPTY)
		password = disassemble_input(ss_inputs);

	else
		throw BadRequest();
}

string Utrip::get_email_address(stringstream& ss_inputs)
{
	string email = disassemble_input(ss_inputs);
	if(!email_is_unique(email))
		throw BadRequest();

	return email;
}

bool Utrip::email_is_unique(string email)
{
	for(auto const& it : users)
		if(it->get_email() == email)
			return false;
	return true;
}

string Utrip::get_username(stringstream& ss_inputs)
{
	string username;
	username = disassemble_input(ss_inputs);
	check_for_username_uniqueness(username);

	return username;
}

void Utrip::check_for_username_uniqueness(string username)
{
	for(auto const& it : users)
		if(it->get_username() == username)
			throw BadRequest();
}

void Utrip::process_login(stringstream& ss_inputs)
{
	string email, password;
	for(int i = 0; i < 2; i++)
		get_user_email_password(email, password, ss_inputs);

	unsigned long int hashed_password = generate_password_hash(password);
	login_user(email, hashed_password);
	cout << OK << endl;
}

void Utrip::get_user_email_password(string& email, string& password, stringstream& ss_inputs)
{
	string command = disassemble_input(ss_inputs);	
	if(command == EMAIL && email == EMPTY)
		email = disassemble_input(ss_inputs);

	else if(command == PASSWORD && password == EMPTY)
		password = disassemble_input(ss_inputs);

	else
		throw BadRequest();
}

void Utrip::login_user(string email, unsigned long int password)
{
	for(auto const& it : users)
		if(it->get_email() == email)
		{
			if(it->get_password() == password)
			{
				it->login();
				return;
			}
		}
	throw BadRequest();
}

void Utrip::process_command_after_GET(stringstream& ss_inputs)
{
	string command = disassemble_input(ss_inputs);
	auto logged_user = find_if(users.begin(), users.end(), logged_in);

	if(logged_user == users.end())
		throw PermissionDenied();
	else
		process_logged_user_requests_GET(ss_inputs, command, *logged_user);
}

void Utrip::process_logged_user_requests_GET(stringstream& ss_inputs,
	 string command, User* logged_user)
{
	if(command == WALLET)
		process_GET_wallet_command(ss_inputs, logged_user);

	else if(command == HOTELS)
		process_GET_hotels_command(ss_inputs, logged_user);

	else if(command == RESERVES)
		process_GET_reserves_command(ss_inputs, logged_user);

	else if(command == COMMENTS)
		process_GET_comments_command(ss_inputs);

	else if(command == RATINGS)
		process_GET_ratings_command(ss_inputs);

	else
		throw NotFound();
}

void Utrip::process_GET_wallet_command(stringstream& ss_inputs,
	 User* logged_user)
{
	check_for_qmark(ss_inputs);
	show_user_recent_transactions(ss_inputs, logged_user);
}

void Utrip::process_GET_hotels_command(stringstream& ss_inputs, User* logged_user)
{
	string command = get_command(ss_inputs);
	if(command == QMARK)
		show_one_hotel_information(ss_inputs);

	else if(command == EMPTY)
		show_all_hotels_information(logged_user);

	else
		throw NotFound();	
}

void Utrip::process_GET_reserves_command(stringstream& ss_inputs,
	 User* logged_user)
{
	string command = get_command(ss_inputs);
	if(command == EMPTY)
		show_user_reservations(logged_user);

	else
		throw NotFound();
}

void Utrip::process_GET_comments_command(stringstream& ss_inputs)
{
	check_for_qmark(ss_inputs);
	show_comments_on_a_hotel(ss_inputs);
}

void Utrip::process_GET_ratings_command(stringstream& ss_inputs)
{
	check_for_qmark(ss_inputs);
	show_ratings_of_a_hotel(ss_inputs);
}

void Utrip::process_wallet_charging(stringstream& ss_inputs, User* logged_user)
{
	double amount = get_amount_input(ss_inputs);
	charge_wallet(amount, logged_user);
}

double Utrip::get_amount_input(stringstream& ss_inputs)
{
	double amount;
	string command = disassemble_input(ss_inputs);
	if(command == AMOUNT)
		amount = get_quantity(ss_inputs);

	else
		throw BadRequest();

	return amount;
}

void Utrip::charge_wallet(double amount, User* logged_user)
{
	logged_user->charge_my_wallet(amount);
	cout << OK << endl;
}

void Utrip::show_user_recent_transactions(stringstream& ss_inputs, User* logged_user)
{
	int count = get_count_input(ss_inputs);
	show_transactions(count, logged_user);
}

int Utrip::get_count_input(stringstream& ss_inputs)
{
	int count;
	string command = disassemble_input(ss_inputs);
	if(command == COUNT)
		count = get_quantity(ss_inputs);

	else
		throw BadRequest();

	return count;
}

void Utrip::check_for_natural_number(double number)
{
	if(number <= 0)
		throw BadRequest();
}

void Utrip::show_transactions(int count, User* logged_user)
{
	logged_user->show_my_recent_transactions(count);
}

void Utrip::show_all_hotels_information(User* logged_user)
{
	hotels_t filtered_hotels = logged_user->apply_criteria(hotels);
	if(filtered_hotels.size() == 0)
		cout << EMPTY_OUTPUT << endl;

	for(auto const& it : filtered_hotels)
		it->print_general_information();
}

void Utrip::show_one_hotel_information(stringstream& ss_inputs)
{
	string id = get_input_hotel_id(ss_inputs, ID);

	auto found_hotel = find_if(hotels.begin(), hotels.end(), MatchingId(id));
	(*found_hotel)->print_specific_information();
}

string Utrip::get_input_hotel_id(stringstream& ss_inputs,
	 const string& defined_command)
{
	string id;
	string command = disassemble_input(ss_inputs);
	if(command == defined_command)
		id = get_hotel_id(ss_inputs);

	else
		throw BadRequest();

	return id;
}

void Utrip::process_reservation(stringstream& ss_inputs, User* logged_user)
{
	ReservationInputs reservation_info;
	for(int i = 0; i < 5; i++)
		get_reservation_inputs(ss_inputs, reservation_info);

	auto found_hotel = find_if(hotels.begin(), hotels.end(), MatchingId(reservation_info.hotel_id));
	rooms_t rooms = (*found_hotel)->choose_room_type(reservation_info);
	(*found_hotel)->check_for_enough_rooms(rooms, reservation_info);

	logged_user->set_user_reservation(*found_hotel, reservation_info);
	rooms_t user_rooms = (*found_hotel)->reserve_room(rooms, reservation_info);
	logged_user->set_user_rooms(user_rooms);
	print_rooms_name(user_rooms);
}

void Utrip::print_rooms_name(rooms_t user_rooms)
{
	cout << user_rooms[0]->get_room_id();
	for(int i = 1; i < user_rooms.size(); i++)
		cout << SPACE << user_rooms[i]->get_room_id();
	cout << endl;
}
void Utrip::get_reservation_inputs(stringstream& ss_inputs, ReservationInputs& reservation_info)
{
	string command = disassemble_input(ss_inputs);
	if(command == HOTEL)
		reservation_info.hotel_id = get_hotel_id(ss_inputs);

	else if(command == TYPE)
		reservation_info.room_type = get_room_type(ss_inputs);
	
	else if(command == QUANTITY)
		reservation_info.quantity = get_quantity(ss_inputs);

	else if(command == CHECK_IN)
		reservation_info.check_in = get_day_of_month(ss_inputs);

	else if(command == CHECK_OUT)
		reservation_info.check_out = get_day_of_month(ss_inputs);

	else
		throw BadRequest();
}

string Utrip::get_hotel_id(stringstream& ss_inputs)
{
	string id = disassemble_input(ss_inputs);
	auto found_hotel = find_if(hotels.begin(), hotels.end(), MatchingId(id));
	if(found_hotel == hotels.end())
		throw NotFound();

	return id;
}

string Utrip::get_room_type(stringstream& ss_inputs)
{
	string room_type;
	room_type = disassemble_input(ss_inputs);
	check_for_valid_room_type(room_type);

	return room_type;
}

void Utrip::check_for_valid_room_type(string room_type)
{
	if(room_type != STANDARD && room_type != DELUX &&
		room_type != LUXURY && room_type != PREMIUM)

	throw NotFound();
}

double Utrip::get_quantity(stringstream& ss_inputs)
{
	double quantity;
	quantity = stod(disassemble_input(ss_inputs));
	check_for_natural_number(quantity);

	return quantity;
}

int Utrip::get_day_of_month(stringstream& ss_inputs)
{
	int day;
	day = stoi(disassemble_input(ss_inputs));

	return day;
}

void Utrip::show_user_reservations(User* logged_user)
{
	logged_user->print_all_reservations();
}

void Utrip::process_command_after_DELETE(stringstream& ss_inputs)
{
	string command = disassemble_input(ss_inputs);
	auto logged_user = find_if(users.begin(), users.end(), logged_in);

	if(logged_user == users.end())
		throw PermissionDenied();
	
	if(command == RESERVES)
	{
		check_for_qmark(ss_inputs);
		process_deletion_of_reservation(ss_inputs, *logged_user);
	}
	else if(command == FILTERS)
		process_deletion_of_filters(ss_inputs, *logged_user);
}

void Utrip::process_deletion_of_reservation(stringstream& ss_inputs, User* logged_user)
{
	int reservation_id = get_reservation_id_input(ss_inputs);
	logged_user->cancel_reservation_by_id(reservation_id);
	cout << OK << endl;
}

int Utrip::get_reservation_id_input(stringstream& ss_inputs)
{
	int reservation_id;
	string command = disassemble_input(ss_inputs);
	if(command == ID)
		reservation_id = stoi(disassemble_input(ss_inputs));

	return reservation_id;
}

void Utrip::process_posting_comment(stringstream& ss_inputs, User* logged_user)
{
	string hotel_id, comment;
	for (int i = 0; i < 2; ++i)
		get_comment_inputs(ss_inputs, hotel_id, comment);

	auto found_hotel = find_if(hotels.begin(), hotels.end(), MatchingId(hotel_id));
	(*found_hotel)->add_comments(logged_user->get_username(), comment);
	cout << OK << endl;
}

void Utrip::get_comment_inputs(stringstream& ss_inputs, string& hotel_id, string& comment)
{
	string command = disassemble_input(ss_inputs);
	if(command == HOTEL)
		hotel_id = get_hotel_id(ss_inputs);

	else if(command == COMMENT)
		comment = disassemble_input(ss_inputs);

	else
		throw BadRequest();
}

void Utrip::show_comments_on_a_hotel(stringstream& ss_inputs)
{
	string hotel_id = get_input_hotel_id(ss_inputs, HOTEL);

	auto found_hotel = find_if(hotels.begin(), hotels.end(), MatchingId(hotel_id));
	(*found_hotel)->show_comments();
}

void Utrip::process_rating(stringstream& ss_inputs, User* logged_user)
{
	string hotel_id;
	Rating user_rating;
	for(int i = 0; i < 7; i++)
		get_rating_inputs(ss_inputs, hotel_id, user_rating);

	auto found_hotel = find_if(hotels.begin(), hotels.end(), MatchingId(hotel_id));
	user_rating.username = logged_user->get_username();
	(*found_hotel)->add_rating(user_rating);
	cout << OK << endl;
}

void Utrip::get_rating_inputs(stringstream& ss_inputs, string& hotel_id,
	 Rating& user_rating)
{
	string command = disassemble_input(ss_inputs);
	if(command == HOTEL)
		hotel_id = get_hotel_id(ss_inputs);

	else if(command == LOCATION)
		user_rating.location = get_rating(ss_inputs);

	else if(command == CLEANLINESS)
		user_rating.cleanliness = get_rating(ss_inputs);

	else if(command == STAFF)
		user_rating.staff = get_rating(ss_inputs);

	else if(command == FACILITIES)
		user_rating.facilities = get_rating(ss_inputs);

	else if(command == VALUE_FOR_MONEY)
		user_rating.value_for_money = get_rating(ss_inputs);

	else if(command == OVERALL_RATING)
		user_rating.overall_rating = get_rating(ss_inputs);

	else
		throw BadRequest();
}

double Utrip::get_rating(stringstream& ss_inputs)
{
	double rating;
	rating = stod(disassemble_input(ss_inputs));
	check_for_valid_rating(rating);

	return rating;
}

void Utrip::check_for_valid_rating(double rating)
{
	if(rating < 1 || rating > 5)
		throw BadRequest();
}

void Utrip::show_ratings_of_a_hotel(stringstream& ss_inputs)
{
	string id = get_input_hotel_id(ss_inputs, HOTEL);

	auto found_hotel = find_if(hotels.begin(), hotels.end(), MatchingId(id));
	(*found_hotel)->show_ratings();
}

void Utrip::process_filtering(stringstream& ss_inputs, User* logged_user)
{
	string command = disassemble_input(ss_inputs);
	if(command == CITY)
		process_city_filtering(ss_inputs, logged_user);

	else if(command == MIN_STAR || command == MAX_STAR)
		process_star_period_filtering(ss_inputs, logged_user);

	else if(command == MIN_PRICE || command == MAX_PRICE)
		process_price_period_filtering(ss_inputs, logged_user);

	else
		process_available_rooms_filtering(ss_inputs, logged_user);
}

void Utrip::process_city_filtering(stringstream& ss_inputs, User* logged_user)
{
	string city = get_city_input(ss_inputs);
	CriteriaCity* criteria = new CriteriaCity(city);
	logged_user->set_criterias(criteria);
	cout << OK << endl;
}

string Utrip::get_city_input(stringstream& ss_inputs)
{
	string city;
	getline(ss_inputs, city);

	if(city == EMPTY)
		throw BadRequest();

	return city;
}

void Utrip::check_for_available_city(string city)
{
	for(const auto& it : cities)
		if(city == it)
			return;

	throw BadRequest();
}

void Utrip::process_star_period_filtering(stringstream& ss_inputs, User* logged_user)
{
	stringstream primary_inputs;
	create_primary_inputs(primary_inputs, ss_inputs);
	int min_star, max_star;
	for(int i = 0; i < 2; i++)
		get_star_period_inputs(primary_inputs, min_star, max_star);

	check_for_valid_period(min_star, max_star);
	apply_star_filtering(min_star, max_star, logged_user);
}

void Utrip::create_primary_inputs(stringstream& primary_inputs,
	 stringstream& ss_inputs)
{
	primary_inputs << ss_inputs.str();
	string dummy;
	getline(primary_inputs, dummy, CHAR_QMARK);
	getline(primary_inputs, dummy, SPACE);
}

void Utrip::get_star_period_inputs(stringstream& ss_inputs, 
	 int& min_star, int& max_star)
{
	string command = disassemble_input(ss_inputs);
	if(command == MIN_STAR)
		min_star = get_rating(ss_inputs);

	else if(command == MAX_STAR)
		max_star = get_rating(ss_inputs);

	else
		throw BadRequest();
}

void Utrip::check_for_valid_period(double start, double end)
{
	if(end < start)
		throw BadRequest();
}

void Utrip::apply_star_filtering(int min_star, int max_star, User* logged_user)
{
	CriteriaStar* criteria = new CriteriaStar(min_star, max_star);
	logged_user->set_criterias(criteria);
	cout << OK << endl;
}

void Utrip::process_price_period_filtering(stringstream& ss_inputs, User* logged_user)
{
	stringstream primary_inputs;
	create_primary_inputs(primary_inputs, ss_inputs);
	double min_price, max_price;
	for(int i = 0; i < 2; i++)
		get_price_period_inputs(primary_inputs, min_price, max_price);

	check_for_valid_period(min_price, max_price);
	apply_price_filtering(min_price, max_price, logged_user);
}

void Utrip::get_price_period_inputs(stringstream& ss_inputs, 
	 double& min_price, double& max_price)
{
	string command = disassemble_input(ss_inputs);
	if(command == MIN_PRICE)
		min_price = get_price(ss_inputs);

	else if(command == MAX_PRICE)
		max_price = get_price(ss_inputs);

	else
		throw BadRequest();
}

double Utrip::get_price(stringstream& ss_inputs)
{
	double price;
	price = stod(disassemble_input(ss_inputs));
	check_for_not_negative(price);

	return price;
}

void Utrip::check_for_not_negative(double number)
{
	if(number < 0)
		throw BadRequest();
}

void Utrip::apply_price_filtering(double min_price, double max_price, User* logged_user)
{
	CriteriaPrice* criteria = new CriteriaPrice(min_price, max_price);
	logged_user->set_criterias(criteria);
	cout << OK << endl;
}

void Utrip::process_available_rooms_filtering(stringstream& ss_inputs, User* logged_user)
{
	stringstream primary_inputs;
	create_primary_inputs(primary_inputs, ss_inputs);
	ReservationInputs filter;
	for(int i = 0; i < 4; i++)
		get_available_rooms_inputs(primary_inputs, filter);

	check_for_valid_period(filter.check_in, filter.check_out);
	apply_available_rooms_filtering(filter, logged_user);
}

void Utrip::get_available_rooms_inputs(stringstream& ss_inputs, ReservationInputs& filter)
{
	string command = disassemble_input(ss_inputs);
	if(command == TYPE)
		filter.room_type = get_room_type(ss_inputs);

	else if(command == QUANTITY)
		filter.quantity = get_quantity(ss_inputs);

	else if(command == CHECK_IN)
		filter.check_in = get_day_of_month(ss_inputs);

	else if(command == CHECK_OUT)
		filter.check_out = get_day_of_month(ss_inputs);

	else
		throw BadRequest();
}

void Utrip::apply_available_rooms_filtering(ReservationInputs filter, User* logged_user)
{
	CriteriaAvailableRooms* criteria = new CriteriaAvailableRooms(filter);
	logged_user->set_criterias(criteria);
	cout << OK << endl;
}

void Utrip::process_deletion_of_filters(stringstream& ss_inputs, User* logged_user)
{
	logged_user->delete_criterias();
	cout << OK << endl;
}
