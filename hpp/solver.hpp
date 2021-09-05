#pragma once
#include "parser.hpp"
#include "static_vector.hpp"
#include <vector>

using numbers_rule = std::array<sudoku_number, SUDOKU_BRD_SIZE>;

struct Node {
  Point pt;
  numbers_rule row, column, window;
};

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

template <typename T, typename C> bool any_of(T arr, C fun) {
  auto comparison = [&](C n) { return n == fun; };
  return std::any_of(begin(arr), end(arr), comparison);
}

template <typename T, typename C> void for_each(T arr, C fun) {
  std::for_each(begin(arr), end(arr), fun);
}

// vector<Point, SUDOKU_AREA_SIZE> gen_points(Sudoku_board const &bd);
static_vector<Point, SUDOKU_AREA_SIZE> gen_points(Sudoku_board const &bd);
std::vector<Possible_number> find_candidates(Sudoku_board const &bd);

numbers_rule gen_row(Sudoku_board const &bd, Point const &pt);
numbers_rule gen_column(Sudoku_board const &bd, Point const &pt);
numbers_rule gen_square(Sudoku_board const &bd, Point const &pt);
bool insert_numbers(Sudoku_board &sb, std::vector<Possible_number> const &pn);

void single_solve(Sudoku_board &sb);

// solving sudoku via brute force search
bool brute_fore_search(Sudoku_board &sb);