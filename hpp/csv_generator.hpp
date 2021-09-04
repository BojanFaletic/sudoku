#pragma once
#include "parser.hpp"
#include <fstream>
#include <iterator>
#include <optional>
#include <string>

using csv_generator = std::optional<std::array<Sudoku_board, 2>>;

class CSV_generator {
public:
  CSV_generator(std::string const &f_name) : file(f_name) {
    std::string st;
    std::getline(file, st);
  }

  csv_generator sample();

private:
  std::ifstream file;
};