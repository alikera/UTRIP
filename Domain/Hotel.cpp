#include "Hotel.hpp"
#include "defines.hpp"
#include "NotEnoughRoom.hpp"
#include <iostream>
#include <vector>
#include <iomanip>

using namespace std;
Hotel::~Hotel()
{
	for(auto p1 : standard_rooms)
		delete p1;
	for(auto p2 : delux_rooms)
		delete p2;
	for(auto p3 : luxury_rooms)
		delete p3;
	for(auto p4 : premium_rooms)
		delete p4;
}

void Hotel::create(string line)
{
	stringstream ss_line(line);
	string item;
	for(int i = 1; getline(ss_line, item, COMMA); i++)
		write_each_hotel_info(i, item);

	rooms_detail.average_price = calculate_average_price_of_rooms();
}

void Hotel::write_each_hotel_info(int index, string item)
{
	switch(index)
	{
		case 1:
			uid = item;
			break;
		case 2:
			name = item;
			break;
		case 3:
			stars = stoi(item);
			break;
		case 4:
			overview = item;
			break;
		case 5:
			amenities = item;
			break;
		case 6:
			city = item;
			break;
		case 7:
			latitude = stod(item);
			break;
		case 8:
			longitude = stod(item);
			break;
		case 9:
			image_url = item;
			break;
		case 10:
			standard_rooms = create_rooms(stoi(item));
			break;
		case 11:
			delux_rooms = create_rooms(stoi(item));
			break;
		case 12:
			luxury_rooms = create_rooms(stoi(item));
			break;
		case 13:
			premium_rooms = create_rooms(stoi(item));
			break;
		case 14:
			set_room_details(standard_rooms,STANDARD_LETTER, stoi(item));
			break;
		case 15:
			set_room_details(delux_rooms,DELUX_LETTER, stoi(item));
			break;
		case 16:
			set_room_details(luxury_rooms,LUXURY_LETTER, stoi(item));
			break;
		default:
			set_room_details(premium_rooms,PREMIUM_LETTER, stoi(item));
	}
}

rooms_t Hotel::create_rooms(int room_count)
{
	rooms_t rooms;
	for(int i = 1; i <= room_count; i++)
	{
		Room* new_room = new Room();
		rooms.push_back(new_room);
	}

	return rooms;
}

void Hotel::set_room_details(rooms_t this_type_rooms, string room_type, int price)
{
	for(int i = 0; i < this_type_rooms.size(); i++)
	{
		string id = room_type + to_string(i+1);
		this_type_rooms[i]->set_room_id(id);
		this_type_rooms[i]->set_price(price);
	}
	set_rooms_price(price, room_type);
}

void Hotel::set_rooms_price(int price ,string room_type)
{
	if(room_type == STANDARD_LETTER)
		rooms_detail.standard_price = price;

	if(room_type == DELUX_LETTER)
		rooms_detail.delux_price = price;

	if(room_type == LUXURY_LETTER)
		rooms_detail.luxury_price= price;

	if(room_type == PREMIUM_LETTER)
		rooms_detail.premium_price = price;
}

void Hotel::print_general_information()
{
	cout << uid << SPACE << name << SPACE
	 	<< stars << SPACE << city << SPACE
	 	<< total_number_of_rooms() << SPACE
	 	<< fixed << setprecision(2) << rooms_detail.average_price << SPACE << endl;
}

int Hotel::total_number_of_rooms()
{
	return standard_rooms.size() + delux_rooms.size() 
		+ luxury_rooms.size() + premium_rooms.size();
}

double Hotel::calculate_average_price_of_rooms()
{
	double sum = calculate_sum_of_each_type_price();
	int zero_counter = count_zeros();
	if(zero_counter == 4)
		return 0;

	return sum / (4 - zero_counter);
}

int Hotel::calculate_sum_of_each_type_price()
{
	return rooms_detail.standard_price +
		rooms_detail.delux_price + 
		rooms_detail.luxury_price +
		rooms_detail.premium_price; 
}

int Hotel::count_zeros()
{
	int counter = 0;
	if(rooms_detail.standard_price == ZERO)
		counter++;

	if(rooms_detail.delux_price == ZERO)
		counter++;

	if(rooms_detail.luxury_price == ZERO)
		counter++;

	if(rooms_detail.premium_price == ZERO)
		counter++;

	return counter;
}

void Hotel::print_specific_information()
{
	cout << uid << endl << name << endl;
	cout << STAR_ << stars << endl;
	cout << OVERVIEW_ << overview << endl;
	cout << AMENITIES_ << amenities << endl;
	cout << CITY_ << city << endl;
	cout << LATITUDE_ << fixed << setprecision(2) << latitude << endl;
	cout << LONGITUDE_ << fixed << setprecision(2) << longitude << endl;
	print_rooms_info();
}

void Hotel::print_rooms_info()
{
	cout << ROOMS_;
	print_count_of_each_type();

	cout << PRICE_;
	print_price_of_rooms();
}

void Hotel::print_count_of_each_type()
{
	cout << standard_rooms.size() << SPACE << delux_rooms.size() << SPACE 
		<< luxury_rooms.size() << SPACE << premium_rooms.size() << endl;
}
void Hotel::print_price_of_rooms()
{
	cout << rooms_detail.standard_price << SPACE
		<< rooms_detail.delux_price << SPACE
		<< rooms_detail.luxury_price << SPACE
		<< rooms_detail.premium_price << endl;
}

rooms_t Hotel::choose_room_type(ReservationInputs reservation_info)
{
	if(reservation_info.room_type == STANDARD)
		return standard_rooms;

	if(reservation_info.room_type == DELUX)
		return delux_rooms;

	if(reservation_info.room_type == LUXURY)
		return luxury_rooms;

	if(reservation_info.room_type == PREMIUM)
		return premium_rooms;
}

rooms_t Hotel::reserve_room(rooms_t this_type, ReservationInputs reservation_info)
{
	rooms_t user_rooms;
	for(auto const& it : this_type)
		if(it->is_reservable(reservation_info.check_in, reservation_info.check_out))
		{
			Room* room = it->reserve(reservation_info.check_in, reservation_info.check_out);
			user_rooms.push_back(room);
			reservation_info.quantity--;
			if(reservation_info.quantity <= 0)
				break;
		}
	return user_rooms;
}

void Hotel::check_for_enough_rooms(rooms_t this_type, ReservationInputs reservation_info)
{
	int available_rooms = 0;
	for(int i = 0; i < this_type.size(); i++)
		if(this_type[i]->is_reservable(reservation_info.check_in, reservation_info.check_out))
			available_rooms++;

	if(available_rooms < reservation_info.quantity)
		throw NotEnoughRoom();
}

int Hotel::get_price_of_this_room_type(string room_type)
{
	if(room_type == STANDARD)
		return standard_rooms[0]->get_price();

	else if(room_type == DELUX)
		return delux_rooms[0]->get_price();

	else if(room_type == LUXURY)
		return luxury_rooms[0]->get_price();

	else if(room_type == PREMIUM)
		return premium_rooms[0]->get_price();
}

void Hotel::add_comments(string username, string comment)
{
	comments.push_front(make_pair(username, comment));
}

void Hotel::show_comments()
{
	stringstream all_comments;
	for(auto const& it : comments)
		all_comments << print_each_comment(it);

	cout << all_comments.str();
}

string Hotel::print_each_comment(comment_t each_comment)
{
	stringstream comment;
	comment << each_comment.first << COLON;
	comment << each_comment.second << endl;
	return comment.str();
}

void Hotel::add_rating(Rating user_rating)
{
	hotel_ratings.set_rating(user_rating);
}

void Hotel::show_ratings()
{
	hotel_ratings.show_GPA_of_ratings();
}