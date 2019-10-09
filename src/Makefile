checkers: checkers.o main.o
	g++ -std=c++14 -Wall -Wextra -pedantic -O3 `pkg-config --cflags gtkmm-3.0` checkers.o main.o -o checkers `pkg-config --libs gtkmm-3.0`

checkers.o: checkers.cpp
	g++ -std=c++14 -Wall -Wextra -pedantic -O3 `pkg-config --cflags gtkmm-3.0` checkers.cpp -c `pkg-config --libs gtkmm-3.0`

main.o: main.cpp checkers.h
	g++ -std=c++14 -Wall -Wextra -pedantic -O3 `pkg-config --cflags gtkmm-3.0` main.cpp -c `pkg-config --libs gtkmm-3.0`

clean:
	@rm -f checkers.o main.o checkers

run:
	@make -s -j2 && ./checkers
