#ifndef USER_HPP
#define USER_HPP
#include <string>
#include <list>
#include <vector>
#include <map>
#include "defines.hpp"
#include "Hotel.hpp"
#include "Criteria.hpp"
#include "SameType.hpp"

typedef std::list<double> transactions_t;
typedef std::list<std::pair<int, rooms_t> > reserves_t;


struct MyReservation
{
	int reservation_id;
	ReservationInputs reservation_info;
	int total_cost;
};

class User
{
public:
	User(std::string email_, std::string username_, unsigned long int password_, int status_);

	std::string get_email() { return email; }
	std::string get_username() { return username; }
	unsigned long int get_password() { return password; }
	int get_status() { return status; }
	double get_my_wallet() { return my_wallet; }

	void login();
	void logout();

	void charge_my_wallet(double amount);
	void show_my_recent_transactions(int count);

	void set_user_reservation(Hotel* found_hotel, ReservationInputs reservation_info);
	void set_user_rooms(rooms_t my_rooms);

	void print_all_reservations();

	void cancel_reservation_by_id(int id);

	void set_criterias(Criteria* criteria);
	hotels_t apply_criteria(hotels_t hotels);
	void delete_criterias();

	void set_default_filter_activation_status(const int& condition);

	void set_manual_weights_condition(const int& condition);
	void set_manual_weights(const map_t& weights);
	int get_manual_weights_condition() { return manual_weights_condition; }
	map_t get_manual_weights() { return manual_weights; }
	void print_manual_weights();

	void add_rating(Rating user_rating);

	map_t get_estimated_weights();

	void print_estimated_weights();
	void check_for_sufficient_ratings();

private:
	std::string email;
	std::string username;
	unsigned long int password;

	int status;
	double my_wallet = 0;
	transactions_t financial_trans {0};

	std::list<MyReservation> my_reservations;
	int reservation_id_counter = 1;
	reserves_t reserved_rooms;

	criterias_t active_criterias;
	int default_filter = ON;
	int manual_weights_condition = OFF;

	map_t manual_weights;

	ratings_t ratings;
	int new_rating;

	map_t estimated_weights;

	void check_for_enough_money(double amount);

	MyReservation set_reservation_info(Hotel* found_hotel, ReservationInputs reservation_info);
	int calculate_total_cost(ReservationInputs reservation_info, int room_price);

	void print_each_reservations_info(const MyReservation& each_reservation, 
		 std::stringstream& reservations_output);

	void cancel_reservation(int id, ReservationInputs to_be_canceled);

	void free_rooms_period(rooms_t rooms, int check_in);
	void take_back_half_money(int total_cost);

	double calculate_standard_deviation();
	std::vector<int> list_reserved_rooms_prices();
	double calculate_average_price();
	double calculate_sigma_of_standard_deviation(const std::vector<int>& prices, 
		 const double& average);

	void active_default_price_filter();
	bool default_filter_cant_be_activate();
	void delete_default_price_filter();
	bool is_default_price_filter(Criteria* this_criteria);

	bool price_filter_is_active(Criteria* this_criteria);
	void create_default_price_filter();

	double truncate(const double& number);

	void estimate_weights();
	int throw_random_number_1_5_();
	void declare_estimated_weights_map(const int& num);
	map_t declare_map_of_derivates(const int& num);

	void reset_derivates(map_t& derivates);
	void calculate_derivates(map_t& derivates);
	void update_estimate_weights(const map_t& derivates);
	void check_for_out_of_range();

	double derivate_error_function(Rating rating, const std::string& key);
	map_t increase_estimated_weights_by_epsilon(const std::string& key);
	double error_function(Rating rating, map_t weights);
	double estimate_rating(Rating rating, map_t weights);

	double calculate_sum_weights_multiply_ratings(const Rating& rating, map_t weights);
	double calculate_sum_of_weights(const map_t& weights);


};

typedef std::list<User*> users_t;

#endif