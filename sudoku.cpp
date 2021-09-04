#include "parser.hpp"
#include "solver.hpp"
#include <iostream>

int main() {
  Sudoku_board sb;

  try {
    sb = parse_input("input.txt");
  } catch (std::runtime_error e) {
    std::cout << e.what();
  }

  std::cout << sb;
  std::cout << sb.num_of_unsolved() << '\n';

  single_solve(sb);

  std::cout << '\n';
  std::cout << sb;
  std::cout << sb.num_of_unsolved() << '\n';

  std::cout << "is board valid: " << is_board_valid(sb) << '\n';

  // test brute force
  brute_fore_search(sb);
  std::cout << '\n';
  std::cout << sb;
  std::cout << sb.num_of_unsolved() << '\n';
  std::cout << "is board valid: " << is_board_valid(sb) << '\n';

  return 0;
}