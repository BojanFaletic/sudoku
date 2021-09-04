#pragma once
#include <array>
#include <string>
#include <fstream>
#include <sstream>
#include <exception>
#include <iostream>
#include <cmath>
#include <algorithm>

using uint = unsigned int;

constexpr uint SUDOKU_BRD_SIZE = 9;
using sudoku_number = unsigned short;
using sudoku_board = std::array<std::array<sudoku_number, SUDOKU_BRD_SIZE>, SUDOKU_BRD_SIZE>;


sudoku_number str_to_num(std::string const &st);
sudoku_board parse_input(std::string const &input_name);
void print_board(sudoku_board const &sb);
uint constexpr sqrt(uint const n);
uint num_of_unsolved(sudoku_board const &sb);