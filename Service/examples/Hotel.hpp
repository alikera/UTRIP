#ifndef HOTEL_HPP
#define HOTEL_HPP
#include "Room.hpp"
#include "Ratings.hpp"
#include "Rating.hpp"
#include "ReservationInputs.hpp"
#include "RoomsDetail.hpp"
#include "Weights.hpp"
#include <vector>
#include <string>
#include <sstream>
#include <list>
#include <map>

typedef std::list<std::pair<std::string, std::string> > list_t;
typedef std::pair<std::string, std::string> comment_t;

typedef std::pair<std::string, double> rate_t;
typedef std::map<std::string, double> map_t;
typedef std::list<map_t> my_ratings_t;
typedef std::vector<std::string> amenities_t;

class Hotel
{
public:
	~Hotel();

	std::string get_uid() { return uid; }
	std::string get_name() { return name; }
	std::string get_city() { return city; }
	std::string get_image_url() { return image_url; }
	int get_stars() { return stars; }
	int get_standard_price() { return rooms_detail.standard_price; }
	int get_delux_price() { return rooms_detail.delux_price; }
	int get_luxury_price() { return rooms_detail.luxury_price; }
	int get_premium_price() { return rooms_detail.premium_price; }
	double get_average_price() { return rooms_detail.average_price; }
	double get_personal_rating() { return personal_rating; }
	int get_uid_generated() { return uid_generated; }
	std::string get_hotel_info_in_html();

	void create(std::string line);

	void print_general_information();

	void print_specific_information();

	rooms_t choose_room_type(ReservationInputs reservation_info);
	void check_for_enough_rooms(rooms_t this_type, ReservationInputs reservation_info);
	rooms_t reserve_room(rooms_t this_type,ReservationInputs reservation_info);
	int get_price_of_this_room_type(std::string room_type);

	void add_comments(std::string username, std::string comment);

	void show_comments();

	void add_rating(Rating user_rating);
	void show_ratings();

	void separate_ratings(std::stringstream& ss_ratings);
	void set_default_ratings(std::vector<double> ratings);

	void show_default_ratings();
	bool check_if_user_has_submitted_rating(std::string username);
	void set_personal_rating(std::string username);

	void apply_weights_for_personal_rating(map_t user_weights);
	amenities_t create_amenities_list(std::string item);
	std::string print_all_amenities();
	std::string create_table();

private:
	std::string uid;
	std::string name;
	int stars;
	std::string overview;
	std::string amenities;
	amenities_t amenities_list;
	std::string city;
	double latitude;
	double longitude;
	std::string image_url;

	rooms_t standard_rooms;
	rooms_t delux_rooms;
	rooms_t luxury_rooms;
	rooms_t premium_rooms;

	RoomsDetail rooms_detail;
	list_t comments;
	Ratings users_ratings;
	map_t default_ratings;

	double personal_rating;
	int uid_generated;

	void write_each_hotel_info(int index, std::string item);
	rooms_t create_rooms(int room_count);
	void set_room_details(rooms_t this_type_rooms, std::string room_type, int price);
	void set_rooms_price(int price ,std::string room_type);

	int total_number_of_rooms();
	double calculate_average_price_of_rooms();
	int calculate_sum_of_each_type_price();
	int count_zeros();

	void print_rooms_info();
	void print_count_of_each_type();
	void print_price_of_rooms();

	std::string print_each_comment(comment_t each_comment);
	double calculate_weighted_sum(map_t user_weights);

	double calculate_weighted_rating(rate_t weight, rate_t rating);

	double calculate_weightes(map_t user_weights);

	static int next_id;
	static int get_next_id() { return next_id++; }
};

typedef std::vector<Hotel*> hotels_t;

#endif