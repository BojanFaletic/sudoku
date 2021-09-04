
HEADERS = $(wildcard hpp/*.hpp)
SRCS = $(wildcard src/*.cpp)
OBJS = $(SRCS:src/%.cpp=obj/%.o)

.PHONY = all, clean


sudoku: sudoku.cpp $(OBJS) $(HEADERS)
	clang++ -std=c++20 -Wall -Wextra -g -Ihpp $< $(OBJS) -o $@


obj/%.o : src/%.cpp
	clang++ -std=c++20 -Wall -Wextra -g -Ihpp -c $< -o $@

clean:
	@rm -f obj/*.o
	@rm -f sudoku