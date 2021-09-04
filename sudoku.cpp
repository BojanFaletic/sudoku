#include "csv_generator.hpp"
#include "parser.hpp"
#include "solver.hpp"
#include <iostream>

int main() {

#if 0
  Sudoku_board sb;
  try {
    sb = parse_input("input.txt");
  } catch (std::runtime_error e) {
    std::cout << e.what();
  }
  std::cout << "Read board\n";
  std::cout << sb;

  brute_fore_search(sb);
  bool is_solved = (is_board_valid(sb) && sb.num_of_unsolved() == 0);
  std::cout << "\nAfter search:" << (is_solved ? "Solved" : "Unsolved") << '\n';
  std::cout << sb;
#endif

  CSV_generator d("sudoku.csv");
  uint score = 0;

  while (true) {
    if (auto s = d.sample()) {
      auto [puzzle, solution] = s.value();

      // try to solve it
      brute_fore_search(puzzle);

      bool is_solved = puzzle == solution;
      if (is_solved) {
        score++;
      } else {
        std::cout << puzzle << '\n';
        std::cout << solution;
        std::cout << "stopped at: " << score << '\n';
        break;
      }
      score += is_solved;
    } else {
      break;
    }
  }

  std::cout << "Final score: " << score << '\n';

  return 0;
}