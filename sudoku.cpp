#include "parser.hpp"
#include <iostream>

int main() {
  sudoku_board sb;

  try {
    sb = parse_input("input.txt");
  } catch (std::runtime_error e) {
    std::cout << e.what();
  }

  print_board(sb);

  return 0;
}