#ifndef SOLVER_HPP
#define SOLVER_HPP

#include "parser.hpp"
#include "static_vector.hpp"
#include "types.hpp"
#include <algorithm>
#include <vector>

template <typename T, typename C> bool any_of(T arr, C fun) {
  auto comparison = [&](C n) { return n == fun; };
  return std::any_of(begin(arr), end(arr), comparison);
}

template <typename T, typename C> void for_each(T arr, C fun) {
  std::for_each_n(begin(arr), arr.size(), fun);
}

static_vector<Possible_number, SUDOKU_AREA_SIZE>
find_candidates(Sudoku_board const &bd);

numbers_rule gen_row(Sudoku_board const &bd, Point const &pt);
numbers_rule gen_column(Sudoku_board const &bd, Point const &pt);
numbers_rule gen_square(Sudoku_board const &bd, Point const &pt);
auto gen_points(Sudoku_board const &bd);
auto gen_all_cases(Sudoku_board const &sb);
bool insert_numbers(Sudoku_board &sb, std::vector<Possible_number> const &pn);

void single_solve(Sudoku_board &sb);

// solving sudoku via brute force search
bool brute_fore_search(Sudoku_board &sb);

#endif
