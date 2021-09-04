#pragma once
#include <algorithm>
#include <array>
#include <cmath>
#include <exception>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

using uint = unsigned int;

constexpr uint SUDOKU_BRD_SIZE = 9;
using sudoku_number = unsigned short;
using sudoku_board =
    std::array<std::array<sudoku_number, SUDOKU_BRD_SIZE>, SUDOKU_BRD_SIZE>;

struct Point {
  uint y, x;

  Point(uint y, uint x) : y{y}, x{x} {}
  Point(Point const &pt) : y{pt.y}, x{pt.x} {}
  ~Point() = default;

  Point &operator*(uint const n) {
    y *= n;
    x *= n;
    return *this;
  }

  Point &operator/(uint const n) {
    y /= n;
    x /= n;
    return *this;
  }

  Point quantize(uint const n) const {
    Point pt{*this};
    return (pt / n) * n;
  }
};

// parsing functions
sudoku_number str_to_num(std::string const &st);
sudoku_board parse_input(std::string const &input_name);

// used for printing
void print_board(sudoku_board const &sb);
uint num_of_unsolved(sudoku_board const &sb);


// checking if board is valid
bool is_row_valid(sudoku_board const &bd, Point const &pt);
bool is_column_valid(sudoku_board const &bd, Point const &pt);
bool is_window_valid(sudoku_board const &bd, Point const &pt);
bool check_all_windows(sudoku_board const &bd);
bool is_board_valid(sudoku_board const &bd);