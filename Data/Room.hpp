#ifndef ROOM_HPP
#define ROOM_HPP
#include <vector>
#include <string>
#include <list>
typedef std::pair<int, int> reservation_period;

class Room
{
public:
	std::string get_room_id() { return room_id; }
	int get_price() { return price; }

	void set_room_id(std::string id);
	void set_price(int price_);

	bool is_reservable(int check_in, int check_out);
	Room* reserve(int check_in, int check_out);

	void free_reservation_period(int check_in);
private:
	std::string room_id;
	int price;
	std::list<reservation_period> periods;
};

typedef std::vector<Room*> rooms_t;

#endif
