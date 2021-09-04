
HEADERS = $(wildcard hpp/*.hpp)
SRCS = $(wildcard src/*.cpp)
OBJS = $(SRCS:src/%.cpp=obj/%.o)

.PHONY = all


sudoku: sudoku.cpp $(OBJS) $(HEADERS)
	clang++ -Wall -Wextra -g -Ihpp $< $(OBJS) -o $@


obj/%.o : src/%.cpp
	clang++ -Wall -Wextra -g -Ihpp -c $< -o $@
