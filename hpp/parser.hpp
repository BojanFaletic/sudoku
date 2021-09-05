#ifndef PARSER_HPP
#define PARSER_HPP

#include "static_vector.hpp"
#include "types.hpp"
#include <algorithm>
#include <array>
#include <cmath>
#include <exception>
#include <fstream>
#include <iostream>
#include <optional>
#include <sstream>
#include <string>

// parsing functions
sudoku_number str_to_num(std::string const &st);
Sudoku_board parse_input(std::string const &input_name);

// checking if board is valid
bool is_row_valid(Sudoku_board const &bd, Point const &pt);
bool is_column_valid(Sudoku_board const &bd, Point const &pt);
bool is_window_valid(Sudoku_board const &bd, Point const &pt);
bool check_all_windows(Sudoku_board const &bd);
bool is_board_valid(Sudoku_board const &bd);

#endif
