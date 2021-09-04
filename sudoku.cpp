#include "ScoreBoard.hpp"
#include "csv_generator.hpp"
#include "parser.hpp"
#include "solver.hpp"
#include <iostream>

#define USE_INPUT_FILE 0

int main() {
#if USE_INPUT_FILE
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

#else

  CSV_generator d("sudoku.csv");
  ScoreBoard sb;

  while (true) {
    auto s = d.sample();
    if (s.has_value()) {
      auto [puzzle, solution] = s.value();

      if (sb.total % 1000 == 0 && sb.total > 0) {
        std::cout << sb.total << " " << sb << '\n';
        break;
      }

      sb.total++;

      // try to solve it
      brute_fore_search(puzzle);

      bool is_solved = puzzle == solution;
      if (is_solved) {
        sb.correct++;
      } else {
        if (false) {
          std::cout << puzzle << '\n';
          std::cout << solution;
          std::cout << "stopped at: " << sb.correct << '\n';
          break;
        }
      }
    } else {
      break;
    }
  }
  std::cout << "Final score: " << sb.correct << '\n';

#endif
  return 0;
}