#include "parser.hpp"
#include "solver.hpp"
#include <iostream>

int main() {
  sudoku_board sb;

  try {
    sb = parse_input("input.txt");
  } catch (std::runtime_error e) {
    std::cout << e.what();
  }

  print_board(sb);
  std::cout << num_of_unsolved(sb) << '\n';

  bool run = true;
  while (run) {
    std::vector<Node> n = gen_nodes(sb);
    std::vector<Possible_number> pn;
    for (Node const &nn : n) {
      pn.push_back(find_candidates(nn));
    }

    run = insert_numbers(sb, pn);
  }
  std::cout << '\n';
  print_board(sb);
  std::cout << num_of_unsolved(sb) << '\n';

  return 0;
}