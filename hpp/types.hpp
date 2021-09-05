#ifndef TYPES_HPP
#define TYPES_HPP

#include "static_vector.hpp"
#include <array>
#include <sstream>

using uint = unsigned int;
using sudoku_number = unsigned short;

constexpr uint SUDOKU_BRD_SIZE = 9;
constexpr uint SUDOKU_AREA_SIZE = SUDOKU_BRD_SIZE * SUDOKU_BRD_SIZE;
using numbers_rule = std::array<sudoku_number, SUDOKU_BRD_SIZE>;

// used for storing y,x position in board
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

struct Node {
  Point pt;
  numbers_rule row, column, window;
};

// used for storing list of possible solution in given square on board
struct Possible_number {
  Point pt;
  static_vector<sudoku_number, SUDOKU_BRD_SIZE> candidate;

  // used for sorting
  bool operator<(Possible_number const &pn) const {
    return candidate.size() < pn.candidate.size();
  }

  friend std::ostream &operator<<(std::ostream &os, Possible_number const &pn) {
    os << pn.pt;
    os << "c: ";
    for (sudoku_number n : pn.candidate) {
      os << n << " ";
    }
    os << '\n';
    return os;
  }
};

// used for storing board and all relevant utility functions
struct Sudoku_board {
  Sudoku_board(Sudoku_board const &) = default;
  Sudoku_board() = default;
  Sudoku_board(Sudoku_board &&) = default;
  Sudoku_board &operator=(Sudoku_board const &sb) = default;

  std::array<std::array<sudoku_number, SUDOKU_BRD_SIZE>, SUDOKU_BRD_SIZE> board;
  static_vector<Possible_number, SUDOKU_AREA_SIZE> points;

  void init_points();

  static constexpr uint size = SUDOKU_BRD_SIZE;

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

#endif
