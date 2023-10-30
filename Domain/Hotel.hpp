#ifndef HOTEL_HPP
#define HOTEL_HPP
#include "Room.hpp"
#include "Ratings.hpp"
#include "Rating.hpp"
#include "ReservationInputs.hpp"
#include "RoomsDetail.hpp"
#include <vector>
#include <string>
#include <sstream>
#include <list>


typedef std::list<std::pair<std::string, std::string> > list_t;
typedef std::pair<std::string, std::string> comment_t;

class Hotel
{
public:
	~Hotel();

	std::string get_uid() { return uid; }
	std::string get_city() { return city; }
	int get_stars() { return stars; }
	double get_average_price() { return rooms_detail.average_price; }

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

private:
	std::string uid;
	std::string name;
	int stars;
	std::string overview;
	std::string amenities;
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
	Ratings hotel_ratings;

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
};

typedef std::vector<Hotel*> hotels_t;

#endif