all: clean bin/warehouse

bin/warehouse: bin/main.o bin/Customer.o bin/Volunteer.o bin/Warehouse.o bin/Order.o bin/Action.o
	g++ -o bin/warehouse bin/main.o bin/Customer.o bin/Volunteer.o bin/Warehouse.o bin/Order.o bin/Action.o

bin/Customer.o: src/Customer.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/Customer.o src/Customer.cpp

bin/Volunteer.o: src/Volunteer.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/Volunteer.o src/Volunteer.cpp

bin/Warehouse.o: src/Warehouse.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/Warehouse.o src/Warehouse.cpp

bin/Order.o: src/Order.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/Order.o src/Order.cpp

bin/Action.o: src/Action.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/Action.o src/Action.cpp

bin/main.o: src/main.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/main.o src/main.cpp

clean:
	rm -f bin/*

--valgrind:
	--valgrind --leak-check=full --show-reachable=yes bin/warehouse
