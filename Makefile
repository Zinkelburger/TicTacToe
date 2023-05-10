CC = g++
CFLAGS = --std=c++17 -Wall -Werror -pedantic -O3
LIB = -lncurses

.PHONY: all clean lint

all: TicTacToe

%.o: %.cpp
	$(CC) $(CFLAGS) -c $<

TicTacToe: main.o tictactoe.o
	$(CC) $(CFLAGS) -o $@ $^ $(LIB)

lint:
	cpplint *.cpp *.hpp

clean:
	rm *.o TicTacToe	
