#ifndef UTTRIP_HPP
#define UTTRIP_HPP

#include "Hotel.hpp"
#include "User.hpp"
#include "defines.hpp"
#include "Rating.hpp"

#include <list>
#include <vector>
#include <set>
#include <string>
#include <sstream>

typedef std::set<std::string> cities_t;

class Utrip
{
public:
	~Utrip();
	void read_from_file(std::string directory);
	void get_requests();

private:
	hotels_t hotels;
	cities_t cities;

	users_t users;

	std::string get_command(std::stringstream& ss_inputs);
	void process_request(std::string inputs);
	void analyze_first_command(std::stringstream& ss_inputs);
	void process_command_after_POST(std::stringstream& ss_inputs);
	void process_signup_or_login_request(std::stringstream& ss_inputs,
	 	 std::string command);
	void process_logged_user_requests_POST(std::stringstream& ss_inputs,
		std::string command, User* logged_user);
	void process_user_practical_requests(std::stringstream& ss_inputs, 
		std::string command, User* logged_user);
	
	void process_signup(std::stringstream& ss_inputs);
	void get_user_information(std::string& email, std::string& username, 
		std::string& password, std::stringstream& ss_inputs);
	std::string get_email_address(std::stringstream& ss_inputs);
	std::string get_username(std::stringstream& ss_inputs);

	void process_login(std::stringstream& ss_inputs);
	void get_user_email_password(std::string& email, std::string& password,
		std::stringstream& ss_inputs);
	void login_user(std::string email, unsigned long int password);
	bool user_logged_in(User* user);
	bool email_is_unique(std::string email);
	void check_for_username_uniqueness(std::string username);

	void process_logout(User* logged_user);

	void process_wallet_charging(std::stringstream& ss_inputs, User* logged_user);
	double get_amount_input(std::stringstream& ss_inputs);
	void charge_wallet(double amount, User* logged_user);

	void process_command_after_GET(std::stringstream& ss_inputs);
	void process_logged_user_requests_GET(std::stringstream& ss_inputs,
		 std::string command, User* logged_user);

	void process_GET_hotels_command(std::stringstream& ss_inputs, User* logged_user);
	void process_GET_reserves_command(std::stringstream& ss_inputs,
	 	 User* logged_user);
	void process_GET_wallet_command(std::stringstream& ss_inputs,
	 	 User* logged_user);
	void process_GET_comments_command(std::stringstream& ss_inputs);
	void process_GET_ratings_command(std::stringstream& ss_inputs);

	void show_user_recent_transactions(std::stringstream& ss_inputs, User* logged_user);
	int get_count_input(std::stringstream& ss_inputs);
	void check_for_natural_number(double number);
	void show_transactions(int count, User* logged_user);

	void show_all_hotels_information(User* logged_user);
	void show_one_hotel_information(std::stringstream& ss_inputs);
	std::string get_input_hotel_id(std::stringstream& ss_inputs,
		 const std::string& defined_command);

	void process_reservation(std::stringstream& ss_inputs, User* logged_user);
	void print_rooms_name(rooms_t user_rooms);
	void get_reservation_inputs(std::stringstream& ss_inputs, 
		 ReservationInputs& reservation_info);
	std::string get_hotel_id(std::stringstream& ss_inputs);
	std::string get_room_type(std::stringstream& ss_inputs);
	double get_quantity(std::stringstream& ss_inputs);
	int get_day_of_month(std::stringstream& ss_inputs);
	void check_for_valid_room_type(std::string room_type);

	void show_user_reservations(User* logged_user);

	void process_command_after_DELETE(std::stringstream& ss_inputs);
	void process_deletion_of_reservation(std::stringstream& ss_inputs, User* logged_user);
	int get_reservation_id_input(std::stringstream& ss_inputs);

	void process_posting_comment(std::stringstream& ss_inputs, User* logged_user);
	void get_comment_inputs(std::stringstream& ss_inputs, std::string& hotel_id, 
		 std::string& comment);
	void show_comments_on_a_hotel(std::stringstream& ss_inputs);

	void process_rating(std::stringstream& ss_inputs, User* logged_user);
	void get_rating_inputs(std::stringstream& ss_inputs, std::string& hotel_id,
		 Rating& user_rating);
	double get_rating(std::stringstream& ss_inputs);
	void check_for_valid_rating(double rating);

	void show_ratings_of_a_hotel(std::stringstream& ss_inputs);

	void process_filtering(std::stringstream& ss_inputs, User* logged_user);
	void process_city_filtering(std::stringstream& ss_inputs, User* logged_user);
	std::string get_city_input(std::stringstream& ss_inputs);
	void check_for_available_city(std::string city);

	void create_primary_inputs(std::stringstream& primary_inputs,
	 	 std::stringstream& ss_inputs);
	void process_star_period_filtering(std::stringstream& ss_inputs, User* logged_user);
	void get_star_period_inputs(std::stringstream& ss_inputs
	 	 ,int& min_star, int& max_star);
	void check_for_valid_period(double start, double end);
	void apply_star_filtering(int min_star, int max_star, User* logged_user);
	void process_price_period_filtering(std::stringstream& ss_inputs, User* logged_user);
	void get_price_period_inputs(std::stringstream& ss_inputs, 
		 double& min_price, double& max_price);
	double get_price(std::stringstream& ss_inputs);
	void check_for_not_negative(double number);
	void apply_price_filtering(double min_price, double max_price, User* logged_user);

	void process_available_rooms_filtering(std::stringstream& ss_inputs, User* logged_user);
	void get_available_rooms_inputs(std::stringstream& ss_inputs, ReservationInputs& filter);
	void apply_available_rooms_filtering(ReservationInputs filter, User* logged_user);
	void process_deletion_of_filters(std::stringstream& ss_inputs, User* logged_user);
};

#endif