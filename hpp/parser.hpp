#pragma once
#include <algorithm>
#include <array>
#include <cmath>
#include <exception>
#include <fstream>
#include <iostream>
#include <optional>
#include <sstream>
#include <string>

using uint = unsigned int;

constexpr uint SUDOKU_BRD_SIZE = 9;
constexpr uint SUDOKU_AREA_SIZE = SUDOKU_BRD_SIZE * SUDOKU_BRD_SIZE;

using sudoku_number = unsigned short;

struct Point {
  uint y, x;

  Point() : y{1000}, x{1000} {}
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

  friend std::ostream &operator<<(std::ostream &os, Point const &pt) {
    os << "(" << pt.y << "y " << pt.x << "x) ";
    return os;
  }
};

struct Sudoku_board {
  Sudoku_board(Sudoku_board const &) = default;
  Sudoku_board() = default;
  Sudoku_board(Sudoku_board &&) = default;
  Sudoku_board &operator=(Sudoku_board const &sb) = default;

  std::array<std::array<sudoku_number, SUDOKU_BRD_SIZE>, SUDOKU_BRD_SIZE> board;
  static constexpr uint size = SUDOKU_BRD_SIZE;

  sudoku_number &operator()(uint i, uint j) { return board[i][j]; }
  sudoku_number &operator()(Point const &pt) { return board[pt.y][pt.x]; }
  sudoku_number operator()(Point const &pt) const { return board[pt.y][pt.x]; }

  uint num_of_unsolved() const;
  bool is_solved() const { return num_of_unsolved() == 0; }

  bool operator==(Sudoku_board const &sb) const {
    for (uint i = 0; i < sb.size; i++) {
      for (uint j = 0; j < sb.size; j++) {
        const Point pt{i, j};
        if (sb(pt) != (*this)(pt))
          return false;
      }
    }
    return true;
  }

  friend std::ostream &operator<<(std::ostream &os, Sudoku_board const &sb);
  auto begin() const { return board.begin(); };
  auto end() const { return board.end(); };
};

// parsing functions
sudoku_number str_to_num(std::string const &st);
Sudoku_board parse_input(std::string const &input_name);

// checking if board is valid
bool is_row_valid(Sudoku_board const &bd, Point const &pt);
bool is_column_valid(Sudoku_board const &bd, Point const &pt);
bool is_window_valid(Sudoku_board const &bd, Point const &pt);
bool check_all_windows(Sudoku_board const &bd);
bool is_board_valid(Sudoku_board const &bd);