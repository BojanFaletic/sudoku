#include "csv_generator.hpp"

csv_generator CSV_generator::sample() {
  std::string line;
  if (!std::getline(file, line))
    return std::nullopt;

  std::string puzzle_s, solution_s;
  std::stringstream ss(line);
  std::getline(ss, puzzle_s, ',');
  std::getline(ss, solution_s, '\n');

  Sudoku_board problem, solution;
  constexpr uint sudoku_area = SUDOKU_BRD_SIZE * SUDOKU_BRD_SIZE;
  if (puzzle_s.size() != sudoku_area || solution_s.size() != sudoku_area)
    return std::nullopt;

  uint idx = 0;
  for (uint i = 0; i < problem.size; i++) {
    for (uint j = 0; j < problem.size; j++) {
      const Point pt{i, j};
      problem(pt) = puzzle_s[idx] - '0';
      solution(pt) = solution_s[idx] - '0';
      idx++;
    }
  }

  return {{problem, solution}};
}