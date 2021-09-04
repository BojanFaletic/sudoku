
HEADERS = $(wildcard hpp/*.hpp)
SRCS = $(wildcard src/*.cpp)
OBJS = $(SRCS:src/%.cpp=obj/%.o)
CFLAGS = -std=c++20 -Wall -Wextra -Ihpp -O3

.PHONY = all, clean


sudoku: sudoku.cpp $(OBJS) $(HEADERS)
	clang++ $(CFLAGS) $< $(OBJS) -o $@


obj/%.o : src/%.cpp
	clang++ $(CFLAGS) -c $< -o $@

clean:
	@rm -f obj/*.o
	@rm -f sudoku