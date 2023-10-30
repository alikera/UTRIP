CC := g++ -std=c++11
obj_files = main.o Utrip.o Hotel.o Room.o functions.o User.o MatchingId.o Ratings.o CriteriaCity.o CriteriaStar.o CriteriaPrice.o CriteriaAvailableRooms.o SameType.o

all: utrip.out

utrip.out: ${obj_files}
	$(CC) ${obj_files} -o utrip.out

main.o: main.cpp Utrip.hpp Hotel.hpp Room.hpp User.hpp functions.hpp
	$(CC) -c main.cpp -o main.o

Utrip.o: Utrip.cpp Utrip.hpp Hotel.hpp User.hpp Room.hpp Rating.hpp functions.hpp BadRequest.hpp NotFound.hpp MatchingId.hpp PermissionDenied.hpp CriteriaCity.hpp CriteriaStar.hpp CriteriaPrice.hpp CriteriaAvailableRooms.hpp
	$(CC) -c Utrip.cpp -o Utrip.o

Hotel.o: Hotel.cpp Hotel.hpp Room.hpp Ratings.hpp defines.hpp ReservationInputs.hpp RoomsDetail.hpp
	$(CC) -c Hotel.cpp -o Hotel.o

Ratings.o: Ratings.cpp Ratings.hpp Rating.hpp NoRating.hpp
	$(CC) -c Ratings.cpp -o Ratings.o

Room.o: Room.cpp Room.hpp defines.hpp
	$(CC) -c Room.cpp -o Room.o

User.o: User.cpp User.hpp Hotel.hpp Criteria.hpp NotFound.hpp SameType.hpp
	$(CC) -c User.cpp -o User.o

functions.o: functions.cpp functions.hpp NotFound.hpp BadRequest.hpp User.hpp Hotel.hpp
	$(CC) -c functions.cpp -o functions.o

MatchingId.o: MatchingId.cpp MatchingId.hpp Hotel.hpp
	$(CC) -c MatchingId.cpp -o MatchingId.o

CriteriaCity.o: CriteriaCity.cpp CriteriaCity.hpp Criteria.hpp Hotel.hpp
	$(CC) -c CriteriaCity.cpp -o CriteriaCity.o

CriteriaStar.o: CriteriaStar.cpp CriteriaStar.hpp Criteria.hpp Hotel.hpp
	$(CC) -c CriteriaStar.cpp -o CriteriaStar.o

CriteriaPrice.o: CriteriaPrice.cpp CriteriaPrice.hpp Criteria.hpp Hotel.hpp
	$(CC) -c CriteriaPrice.cpp -o CriteriaPrice.o

CriteriaAvailableRooms.o: CriteriaAvailableRooms.cpp CriteriaAvailableRooms.hpp Criteria.hpp Hotel.hpp ReservationInputs.hpp
	$(CC) -c CriteriaAvailableRooms.cpp -o CriteriaAvailableRooms.o

SameType.o: SameType.cpp SameType.hpp Criteria.hpp
	$(CC) -c SameType.cpp -o SameType.o

.PHONY: clean
clean:
	rm -r *.o
