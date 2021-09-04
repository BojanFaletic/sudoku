#pragma once
#include "parser.hpp"
#include <vector>

using numbers_rule = std::array<sudoku_number, SUDOKU_BRD_SIZE>;

struct Node {
  Point pt;
  numbers_rule row, column, window;
};

struct Possible_number {
  Point pt;
  std::vector<sudoku_number> candidate;

  // used for sorting
  bool operator<(Possible_number const &pn){
    return candidate.size() < pn.candidate.size();
  }
};

template <typename T, typename C> bool any_of(T arr, C fun) {
  auto comparison = [&](C n) { return n == fun; };
  return std::any_of(begin(arr), end(arr), comparison);
}

std::vector<Node> gen_nodes(sudoku_board const &bd);
Possible_number find_candidates(Node const &nd);

numbers_rule gen_row(sudoku_board const &bd, Point const &pt);
numbers_rule gen_column(sudoku_board const &bd, Point const &pt);
numbers_rule gen_square(sudoku_board const &bd, Point const &pt);
bool insert_numbers(sudoku_board &sb, std::vector<Possible_number> const &pn);