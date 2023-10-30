CC=g++
STD=-std=c++11 -Wall -pedantic
CF=$(STD)
BUILD_DIR=build

all: $(BUILD_DIR) myserver.out

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/Utrip.o: examples/Utrip.cpp examples/Utrip.hpp examples/Hotel.hpp examples/User.hpp examples/Room.hpp examples/Rating.hpp examples/functions.hpp examples/BadRequest.hpp examples/NotFound.hpp examples/MatchingId.hpp examples/PermissionDenied.hpp examples/CriteriaCity.hpp examples/CriteriaStar.hpp examples/CriteriaPrice.hpp examples/CriteriaAvailableRooms.hpp examples/InsufficientRatings.hpp examples/Order.hpp
	$(CC) $(CF) -c examples/Utrip.cpp -o $(BUILD_DIR)/Utrip.o

$(BUILD_DIR)/Hotel.o: examples/Hotel.cpp examples/Hotel.hpp examples/Room.hpp examples/Ratings.hpp examples/defines.hpp examples/ReservationInputs.hpp examples/RoomsDetail.hpp examples/Weights.hpp
	$(CC) $(CF) -c examples/Hotel.cpp -o $(BUILD_DIR)/Hotel.o

$(BUILD_DIR)/Ratings.o: examples/Ratings.cpp examples/Ratings.hpp examples/Rating.hpp examples/NoRating.hpp
	$(CC) $(CF) -c examples/Ratings.cpp -o $(BUILD_DIR)/Ratings.o

$(BUILD_DIR)/Room.o: examples/Room.cpp examples/Room.hpp examples/defines.hpp
	$(CC) $(CF) -c examples/Room.cpp -o $(BUILD_DIR)/Room.o

$(BUILD_DIR)/User.o: examples/User.cpp examples/User.hpp examples/Hotel.hpp examples/Criteria.hpp examples/NotFound.hpp examples/SameType.hpp examples/CriteriaPrice.hpp examples/CriteriaDefault.hpp examples/Weights.hpp
	$(CC) $(CF) -c examples/User.cpp -o $(BUILD_DIR)/User.o

$(BUILD_DIR)/functions.o: examples/functions.cpp examples/functions.hpp examples/NotFound.hpp examples/BadRequest.hpp examples/User.hpp examples/Hotel.hpp
	$(CC) $(CF) -c examples/functions.cpp -o $(BUILD_DIR)/functions.o

$(BUILD_DIR)/MatchingId.o: examples/MatchingId.cpp examples/MatchingId.hpp examples/Hotel.hpp
	$(CC) $(CF) -c examples/MatchingId.cpp -o $(BUILD_DIR)/MatchingId.o

$(BUILD_DIR)/CriteriaCity.o: examples/CriteriaCity.cpp examples/CriteriaCity.hpp examples/Criteria.hpp examples/Hotel.hpp
	$(CC) $(CF) -c examples/CriteriaCity.cpp -o $(BUILD_DIR)/CriteriaCity.o

$(BUILD_DIR)/CriteriaStar.o: examples/CriteriaStar.cpp examples/CriteriaStar.hpp examples/Criteria.hpp examples/Hotel.hpp
	$(CC) $(CF) -c examples/CriteriaStar.cpp -o $(BUILD_DIR)/CriteriaStar.o

$(BUILD_DIR)/CriteriaPrice.o: examples/CriteriaPrice.cpp examples/CriteriaPrice.hpp examples/Criteria.hpp examples/Hotel.hpp
	$(CC) $(CF) -c examples/CriteriaPrice.cpp -o $(BUILD_DIR)/CriteriaPrice.o

$(BUILD_DIR)/CriteriaAvailableRooms.o: examples/CriteriaAvailableRooms.cpp examples/CriteriaAvailableRooms.hpp examples/Criteria.hpp examples/Hotel.hpp examples/ReservationInputs.hpp
	$(CC) $(CF) -c examples/CriteriaAvailableRooms.cpp -o $(BUILD_DIR)/CriteriaAvailableRooms.o

$(BUILD_DIR)/CriteriaDefault.o: examples/CriteriaDefault.cpp examples/CriteriaDefault.hpp examples/Criteria.hpp examples/Hotel.hpp
	$(CC) $(CF) -c examples/CriteriaDefault.cpp -o $(BUILD_DIR)/CriteriaDefault.o

$(BUILD_DIR)/SameType.o: examples/SameType.cpp examples/SameType.hpp examples/Criteria.hpp
	$(CC) $(CF) -c examples/SameType.cpp -o $(BUILD_DIR)/SameType.o

$(BUILD_DIR)/Order.o: examples/Order.cpp examples/Order.hpp examples/Hotel.hpp examples/defines.hpp
	$(CC) $(CF) -c examples/Order.cpp -o $(BUILD_DIR)/Order.o

$(BUILD_DIR)/response.o: utils/response.cpp utils/response.hpp utils/include.hpp
	$(CC) $(CF) -c utils/response.cpp -o $(BUILD_DIR)/response.o

$(BUILD_DIR)/request.o: utils/request.cpp utils/request.hpp utils/include.hpp utils/utilities.hpp
	$(CC) $(CF) -c utils/request.cpp -o $(BUILD_DIR)/request.o

$(BUILD_DIR)/utilities.o: utils/utilities.cpp utils/utilities.hpp
	$(CC) $(CF) -c utils/utilities.cpp -o $(BUILD_DIR)/utilities.o

$(BUILD_DIR)/server.o: server/server.cpp server/server.hpp server/route.hpp utils/utilities.hpp utils/response.hpp utils/request.hpp utils/include.hpp
	$(CC) $(CF) -c server/server.cpp -o $(BUILD_DIR)/server.o

$(BUILD_DIR)/route.o: server/route.cpp server/route.hpp utils/utilities.hpp utils/response.hpp utils/request.hpp utils/include.hpp
	$(CC) $(CF) -c server/route.cpp -o $(BUILD_DIR)/route.o

$(BUILD_DIR)/Welcomehandler.o: examples/Welcomehandler.cpp server/server.hpp utils/utilities.hpp utils/response.hpp utils/request.hpp utils/include.hpp examples/Utrip.hpp examples/defineshandlers.hpp
	$(CC) $(CF) -c examples/Welcomehandler.cpp -o $(BUILD_DIR)/Welcomehandler.o

$(BUILD_DIR)/Signuphandlers.o: examples/Signuphandlers.cpp server/server.hpp utils/utilities.hpp utils/response.hpp utils/request.hpp utils/include.hpp examples/Utrip.hpp examples/defineshandlers.hpp
	$(CC) $(CF) -c examples/Signuphandlers.cpp -o $(BUILD_DIR)/Signuphandlers.o

$(BUILD_DIR)/Loginhandlers.o: examples/Loginhandlers.cpp server/server.hpp utils/utilities.hpp utils/response.hpp utils/request.hpp utils/include.hpp examples/Utrip.hpp examples/defineshandlers.hpp
	$(CC) $(CF) -c examples/Loginhandlers.cpp -o $(BUILD_DIR)/Loginhandlers.o

$(BUILD_DIR)/Logouthandler.o: examples/Logouthandler.cpp server/server.hpp utils/utilities.hpp utils/response.hpp utils/request.hpp utils/include.hpp examples/Utrip.hpp examples/defineshandlers.hpp
	$(CC) $(CF) -c examples/Logouthandler.cpp -o $(BUILD_DIR)/Logouthandler.o

$(BUILD_DIR)/Homehandlers.o: examples/Homehandlers.cpp server/server.hpp utils/utilities.hpp utils/response.hpp utils/request.hpp utils/include.hpp examples/Utrip.hpp examples/defineshandlers.hpp
	$(CC) $(CF) -c examples/Homehandlers.cpp -o $(BUILD_DIR)/Homehandlers.o

$(BUILD_DIR)/Wallethandlers.o: examples/Wallethandlers.cpp server/server.hpp utils/utilities.hpp utils/response.hpp utils/request.hpp utils/include.hpp examples/Utrip.hpp examples/defineshandlers.hpp
	$(CC) $(CF) -c examples/Wallethandlers.cpp -o $(BUILD_DIR)/Wallethandlers.o

$(BUILD_DIR)/main.o: examples/main.cpp server/server.hpp utils/utilities.hpp utils/response.hpp utils/request.hpp utils/include.hpp examples/Utrip.hpp server/server.hpp examples/defineshandlers.hpp
	$(CC) $(CF) -c examples/main.cpp -o $(BUILD_DIR)/main.o

myserver.out: $(BUILD_DIR)/main.o $(BUILD_DIR)/Loginhandlers.o $(BUILD_DIR)/Logouthandler.o $(BUILD_DIR)/Welcomehandler.o $(BUILD_DIR)/Wallethandlers.o $(BUILD_DIR)/Homehandlers.o $(BUILD_DIR)/Signuphandlers.o $(BUILD_DIR)/response.o $(BUILD_DIR)/request.o $(BUILD_DIR)/utilities.o $(BUILD_DIR)/server.o $(BUILD_DIR)/route.o $(BUILD_DIR)/Utrip.o $(BUILD_DIR)/Hotel.o $(BUILD_DIR)/Room.o $(BUILD_DIR)/functions.o $(BUILD_DIR)/User.o $(BUILD_DIR)/MatchingId.o $(BUILD_DIR)/Ratings.o $(BUILD_DIR)/CriteriaCity.o $(BUILD_DIR)/CriteriaStar.o $(BUILD_DIR)/CriteriaPrice.o $(BUILD_DIR)/CriteriaAvailableRooms.o $(BUILD_DIR)/SameType.o $(BUILD_DIR)/Order.o $(BUILD_DIR)/CriteriaDefault.o
	$(CC) $(CF) $(BUILD_DIR)/main.o $(BUILD_DIR)/Loginhandlers.o $(BUILD_DIR)/Logouthandler.o $(BUILD_DIR)/Welcomehandler.o $(BUILD_DIR)/Wallethandlers.o $(BUILD_DIR)/Homehandlers.o $(BUILD_DIR)/Signuphandlers.o $(BUILD_DIR)/response.o $(BUILD_DIR)/request.o $(BUILD_DIR)/utilities.o $(BUILD_DIR)/server.o $(BUILD_DIR)/route.o $(BUILD_DIR)/Utrip.o $(BUILD_DIR)/Hotel.o $(BUILD_DIR)/Room.o $(BUILD_DIR)/functions.o $(BUILD_DIR)/User.o $(BUILD_DIR)/MatchingId.o $(BUILD_DIR)/Ratings.o $(BUILD_DIR)/CriteriaCity.o $(BUILD_DIR)/CriteriaStar.o $(BUILD_DIR)/CriteriaPrice.o $(BUILD_DIR)/CriteriaAvailableRooms.o $(BUILD_DIR)/SameType.o $(BUILD_DIR)/Order.o $(BUILD_DIR)/CriteriaDefault.o -o myserver.out

.PHONY: clean
clean:
	rm -rf $(BUILD_DIR) *.o *.out &> /dev/null
