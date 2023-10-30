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
#include "Order.hpp"
#include "Order.cpp"

#include <iostream>
#include <sstream>
#include <fstream>
#include <iterator>
#include <algorithm>
#include <random>

using namespace std;

Utrip::~Utrip()
{
	for(auto hotel : hotels)
		delete hotel;
	for(auto user : users)
		delete user;
}

void Utrip::read_from_files(string hotels_directory, string ratings_directory)
{
	read_from_hotels_file(hotels_directory);
	read_from_ratings_file(ratings_directory);
}

void Utrip::read_from_hotels_file(string directory)
{
	ifstream hotels_file(directory);
	string line, item;
	getline(hotels_file, line);
	while(getline(hotels_file, line))
	{
		Hotel* each_hotel = new Hotel;
		each_hotel->create(line);
		hotels.push_back(each_hotel);
	}
}

void Utrip::read_from_ratings_file(string directory)
{
	ifstream ratings_file(directory);
	string line, item, hotel_id;
	getline(ratings_file, line);
	while(getline(ratings_file, line))
	{
		stringstream ss_ratings(line);

		getline(ss_ratings, hotel_id, COMMA);
		auto found_hotel = find_if(hotels.begin(), hotels.end(), MatchingId(hotel_id));

		(*found_hotel)->separate_ratings(ss_ratings);
	}
}

void Utrip::get_requests()
{
	string inputs;
	while(getline(cin, inputs))
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
	if(command == PPOST)
		process_command_after_POST(ss_inputs);

	else if(command == GGET)
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
	sort(hotels.begin(), hotels.end(), compare_uid);
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
		process_logout(logged_user->get_username());

	else if(command == SIGNUP)
		throw BadRequest();
	else if(command == LOGIN)
		throw BadRequest();

	else
		process_user_practical_requests(ss_inputs, command, logged_user);
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

	else if(command == DEFAULT_PRICE_FILTER)
		process_default_price_filtering(ss_inputs, logged_user);

	else if(command == SORT)
		process_sorting_hotels(ss_inputs);

	else if(command == MANUAL_WEIGHTS)
		process_manual_weighting(ss_inputs, logged_user);

	else
		throw NotFound();
}

void Utrip::process_signup(stringstream& ss_inputs)
{
	string email, username, password;
	for(int i = 0; i < 3; i++)
		get_user_information(email, username, password, ss_inputs);

	create_user(username, password, email);
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

string Utrip::get_username(stringstream& ss_inputs)
{
	string username;
	username = disassemble_input(ss_inputs);
	username_is_unique(username);

	return username;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Utrip::create_user(const string& username, const string& password, 
	 const string& email)
{	
	unsigned long int hashed_password = generate_password_hash(password);
	User* new_user = new User(email, username, hashed_password, LOGGED_OUT);
	users.push_back(new_user);
}

bool Utrip::have_empty_field(const string& username, const string& password, 
	 const string& email)
{
	if(username == EMPTY || password == EMPTY || email == EMPTY)
		return true;
}

bool Utrip::username_and_email_are_unique(const string& username, const string& email)
{
	if(!email_is_unique(email))
		return false;

	if(!username_is_unique(username))
		return false;

	return true;
}

bool Utrip::email_is_unique(const string& email)
{
	for(auto const& user : users)
		if(user->get_email() == email)
			return false;

	return true;
}

bool Utrip::username_is_unique(const string& username)
{
	for(auto const& user : users)
		if(user->get_username() == username)
			return false;

	return true;
}

Hotel* Utrip::get_hotel(const int& id)
{
	auto hotel = find_if(hotels.begin(), hotels.end(), [id](Hotel* h) 
	{ 
		return h->get_uid_generated() == id;
	});

	return *hotel;
}

void Utrip::apply_star_filter(const int& min_star, const int& max_star, 
	 const string& session_id)
{
	auto logged_user = find_if(users.begin(), users.end(), [session_id](User* user)
	{
		return user->get_username() == session_id;
	});

	CriteriaStar* criteria = new CriteriaStar(min_star, max_star);
	(*logged_user)->set_criterias(criteria);
}

hotels_t Utrip::get_all_hotels(const string& session_id)
{
	auto logged_user = find_if(users.begin(), users.end(), [session_id](User* user)
	{
		return user->get_username() == session_id;
	});

	return (*logged_user)->apply_criteria(hotels);

}
double Utrip::get_user_wallet_credit(string session_id)
{
	auto logged_user = find_if(users.begin(), users.end(), [session_id](User* user)
	{
		return user->get_username() == session_id;
	});

	return (*logged_user)->get_my_wallet();
}
 
bool Utrip::user_is_logged_in(string session_id)
{
	for(auto user : users)
		if(user->get_username() == session_id)
			return true;
	return false;
}

void Utrip::process_logout(const string& session_id)
{
	auto logged_user = find_if(users.begin(), users.end(), [session_id](User* user)
	{
		return user->get_username() == session_id;
	});	
	(*logged_user)->logout();
	(*logged_user)->set_manual_weights_condition(OFF);
}

bool Utrip::login_user(string username, string password)
{
	unsigned long int hashed_password = generate_password_hash(password);

	for(auto const& it : users)
		if(it->get_username() == username)
			if(it->get_password() == hashed_password)
			{
				it->login();
				return true;
			}

	return false;
}

void Utrip::charge_user_wallet_credit(double amount, string session_id)
{
	auto logged_user = find_if(users.begin(), users.end(), [session_id](User* user)
	{
		return user->get_username() == session_id;
	});

	(*logged_user)->charge_my_wallet(amount);
}









void Utrip::process_login(stringstream& ss_inputs)
{
	string email, password;
	for(int i = 0; i < 2; i++)
		get_user_email_password(email, password, ss_inputs);

	login_user(email, password);
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

	else if(command == MANUAL_WEIGHTS)
		process_GET_manual_weights(ss_inputs, logged_user);

	else if(command == ESTIMATED_WEIGHTS)
		process_GET_estimated_weights(ss_inputs, logged_user);

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

	set_hotel_new_ratings(user_rating, hotel_id, logged_user);
	set_user_ratings(user_rating, hotel_id, logged_user);
	cout << OK << endl;
}

void Utrip::set_hotel_new_ratings(Rating user_rating, const string& hotel_id, 
	 User* logged_user)
{
	auto found_hotel = find_if(hotels.begin(), hotels.end(), MatchingId(hotel_id));
	user_rating.id = logged_user->get_username();
	(*found_hotel)->add_rating(user_rating);
}

void Utrip::set_user_ratings(Rating user_rating,  const string& hotel_id, 
	 User* logged_user)
{
	user_rating.id = hotel_id;
	logged_user->add_rating(user_rating);
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

void Utrip::process_default_price_filtering(stringstream& ss_inputs, User* logged_user)
{
	string command = disassemble_input(ss_inputs);
	if(command == ACTIVE)
		process_default_filter_activation(ss_inputs, logged_user);
	else
		throw BadRequest();
}

void Utrip::process_default_filter_activation(stringstream& ss_inputs, User* logged_user)
{
	string status = disassemble_input(ss_inputs);
	if(status == TRUEE)
		logged_user->set_default_filter_activation_status(ON);

	else if(status == FALSEE)
		logged_user->set_default_filter_activation_status(OFF);

	else
		throw BadRequest();

	cout << OK << endl;
}

void Utrip::process_sorting_hotels(stringstream& ss_inputs)
{
	string property, order;
	for(int i = 0; i < 2; i++)
		get_sorting_inputs(ss_inputs, property, order);

	apply_sorting(property, order);
}

void Utrip::get_sorting_inputs(stringstream& ss_inputs, 
	 string& property, string& order)
{
	string command = disassemble_input(ss_inputs);	
	if(command == PROPERTY)
		property = disassemble_input(ss_inputs);

	else if(command == ORDER)
		order = disassemble_input(ss_inputs);

	else
		throw BadRequest();
}

void Utrip::apply_sorting(const string& property, const string& order)
{
	if(order == ASCENDING || order == DESCENDING)
		process_sorting(property, order);

	else
		throw BadRequest();

	cout << OK << endl;
}

void Utrip::process_sorting(const string& property, const string& order)
{
	if(property == ID)
		sort_by_id(order);

	else if(property == NAME)
		sort_by_name(order);

	else if(property == STAR_RATING)
		sort_by_stars(order);

	else if(property == CITY)
		sort_by_city(order);

	else if(property == STANDARD_ROOM_PRICE)
		sort_by_standard_room_price(order);

	else if(property == DELUX_ROOM_PRICE)
		sort_by_delux_room_price(order);

	else if(property == LUXURY_ROOM_PRICE)
		sort_by_luxury_room_price(order);

	else if(property == PREMIUM_ROOM_PRICE)
		sort_by_premium_room_price(order);

	else if(property == AVERAGE_ROOM_PRICE)
		sort_by_average_room_price(order);

	else if(property == RATING_PERSONAL)
		turn_personal_rating_sort_on(order);

	else
		throw BadRequest();
}

void Utrip::sort_by_id(const string& order)
{
	sort(hotels.begin(), hotels.end(), [=](Hotel* h1, Hotel* h2) 
	{
		Order<std::string> compare(h1, h2, &Hotel::get_uid, order);
		return compare();
	});
	personal_rating_sort = OFF;
}

void Utrip::sort_by_name(const string& order)
{
	sort(hotels.begin(), hotels.end(), [=](Hotel* h1, Hotel* h2) 
	{
		Order<std::string> compare(h1, h2, &Hotel::get_name, order);
		return compare();
	});
	personal_rating_sort = OFF;
}

void Utrip::sort_by_stars(const string& order)
{
	sort(hotels.begin(), hotels.end(), [=](Hotel* h1, Hotel* h2)	
	{
		Order<int> compare(h1, h2, &Hotel::get_stars, order);
		return compare();
	});
	personal_rating_sort = OFF;
}

void Utrip::sort_by_city(const string& order)
{
	sort(hotels.begin(), hotels.end(), [=](Hotel* h1, Hotel* h2)
	{
		Order<std::string> compare(h1, h2, &Hotel::get_city, order);
		return compare();
	});
	personal_rating_sort = OFF;
}

void Utrip::sort_by_standard_room_price(const string& order)
{
	sort(hotels.begin(), hotels.end(), [=](Hotel* h1, Hotel* h2)
	{
		Order<int> compare(h1, h2, &Hotel::get_standard_price, order);
		return compare();

	});
	personal_rating_sort = OFF;	
}

void Utrip::sort_by_delux_room_price(const string& order)
{
	sort(hotels.begin(), hotels.end(), [=](Hotel* h1, Hotel* h2)
	{
		Order<int> compare(h1, h2, &Hotel::get_delux_price, order);
		return compare();
	});
	personal_rating_sort = OFF;	
}

void Utrip::sort_by_luxury_room_price(const string& order)
{
	sort(hotels.begin(), hotels.end(), [=](Hotel* h1, Hotel* h2)
	{
		Order<int> compare(h1, h2, &Hotel::get_luxury_price, order);
		return compare();
	});
	personal_rating_sort = OFF;
}

void Utrip::sort_by_premium_room_price(const string& order)
{
	sort(hotels.begin(), hotels.end(), [=](Hotel* h1, Hotel* h2)
	{
		Order<int> compare(h1, h2, &Hotel::get_premium_price, order);
		return compare();
	});
	personal_rating_sort = OFF;
}

void Utrip::sort_by_average_room_price(const string& order)
{
	sort(hotels.begin(), hotels.end(), [=](Hotel* h1, Hotel* h2)
	{
		Order<double> compare(h1, h2, &Hotel::get_average_price, order);
		return compare();
	});
	personal_rating_sort = OFF;
}

void Utrip::sort_by_personal_rating(const string& order)
{
	sort(hotels.begin(), hotels.end(), [=](Hotel* h1, Hotel* h2)
	{
		Order<double> compare(h1, h2, &Hotel::get_personal_rating, order);
		return compare();
	});	
}

void Utrip::turn_personal_rating_sort_on(const string& order)
{
	personal_rating_sort = ON;
	order_type = order;
}

void Utrip::apply_personal_rating_sort()
{
	auto logged_user = find_if(users.begin(), users.end(), logged_in);
	if((*logged_user)->get_manual_weights_condition() == ON)
		apply_user_manual_weights(*logged_user);

	else
		apply_estimated_weights(*logged_user);

	sort_by_personal_rating(order_type);
}

void Utrip::apply_user_manual_weights(User* logged_user)
{
	map_t manual_weights = logged_user->get_manual_weights();
	set_hotels_personal_rating(logged_user, manual_weights);
}

void Utrip::apply_estimated_weights(User* logged_user)
{
	logged_user->check_for_sufficient_ratings();
	map_t estimated_weights = logged_user->get_estimated_weights();
	set_hotels_personal_rating(logged_user, estimated_weights);

}

void Utrip::set_hotels_personal_rating(User* logged_user, map_t weights)
{
	string username = logged_user->get_username();
	set_personal_rating_as_user_overall_rating(username);

	set_personal_rating_with_weights(logged_user, username, weights);	
}

void Utrip::set_personal_rating_as_user_overall_rating(const string& username)
{
	for(auto hotel : hotels)
		if(hotel->check_if_user_has_submitted_rating(username))
			hotel->set_personal_rating(username);	
}

void Utrip::set_personal_rating_with_weights(User* logged_user,
	 const string& username, map_t weights)
{
	for(auto hotel : hotels)
		if(!hotel->check_if_user_has_submitted_rating(username))
			hotel->apply_weights_for_personal_rating(weights);		
}

void Utrip::process_manual_weighting(stringstream& ss_inputs, User* logged_user)
{
	map_t weights;
	for(int i = 0; i < 6; i++){
		if(get_weights_input(ss_inputs, weights, logged_user)) return;
	}

	active_manual_weighting(weights, logged_user);
	cout << OK << endl;
}

bool Utrip::get_activation_input(stringstream& ss_inputs, User* logged_user)
{
	string activation = disassemble_input(ss_inputs);

	if(activation == TRUEE)
		return false;

	else if(activation == FALSEE)
	{
		logged_user->set_manual_weights_condition(OFF);
		cout << OK << endl;
		return true;
	}

	else
		throw BadRequest();
}

bool Utrip::get_weights_input(stringstream& ss_inputs, map_t& weights,
	 User* logged_user)
{
	string command = disassemble_input(ss_inputs);
	if(command == LOCATION)
		weights[_LOCATION_] = get_rating(ss_inputs);

	else if(command == FACILITIES)
		weights[_FACILITIES_] = get_rating(ss_inputs);

	else if(command == CLEANLINESS)
		weights[_CLEANLINESS_] = get_rating(ss_inputs);

	else if(command == STAFF)
		weights[_STAFF_] = get_rating(ss_inputs);

	else if(command == VALUE_FOR_MONEY)
		weights[_VALUE_FOR_MONEY_] = get_rating(ss_inputs);

	else if(command == ACTIVE)
		return get_activation_input(ss_inputs, logged_user);

	else
		throw BadRequest();

	return false;
}

void Utrip::active_manual_weighting(map_t weights, User* logged_user)
{
	logged_user->set_manual_weights_condition(ON);
	logged_user->set_manual_weights(weights);
}

void Utrip::process_GET_manual_weights(stringstream& ss_inputs, User* logged_user)
{
	if(logged_user->get_manual_weights_condition() == OFF)
	{
		cout << ACTIVE << SPACE << FALSEE << endl;
		return;
	}

	logged_user->print_manual_weights();
}

void Utrip::process_GET_estimated_weights(stringstream& ss_inputs, User* logged_user)
{
	logged_user->print_estimated_weights();
}
