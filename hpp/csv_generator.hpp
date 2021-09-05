#ifndef CSV_GENERATOR_HPP
#define CSV_GENERATOR_HPP

#include "types.hpp"
#include <fstream>
#include <optional>
#include <string>

using csv_generator = std::optional<std::array<Sudoku_board, 2>>;

// used for generating sample from csv file with sudoku puzzles (from kaggle)
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

#endif
