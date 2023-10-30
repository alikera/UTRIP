#ifndef USER_HPP
#define USER_HPP
#include <string>
#include <list>
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

	void check_for_enough_money(double amount);

	MyReservation set_reservation_info(Hotel* found_hotel, ReservationInputs reservation_info);
	int calculate_total_cost(ReservationInputs reservation_info, int room_price);

	void print_each_reservations_info(const MyReservation& each_reservation, 
		 std::stringstream& reservations_output);

	void cancel_reservation(int id, ReservationInputs to_be_canceled);

	void free_rooms_period(rooms_t rooms, int check_in);
	void take_back_half_money(int total_cost);
};

typedef std::list<User*> users_t;

#endif