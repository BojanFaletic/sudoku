#include "parser.hpp"

sudoku_number str_to_num(std::string const &st) {
  if (st.size() != 1)
    throw std::runtime_error("Unable to decode number\n");

  const char c_num = st[0];
  if (c_num >= '1' && c_num <= '9')
    return c_num - '0';
  if (c_num == 'x' || c_num == 'X')
    return 0;

  throw std::runtime_error("Number is not correct\n");
}

sudoku_board parse_input(std::string const &input_name) {
  std::ifstream file(input_name);

  if (!file.is_open()) {
    throw std::runtime_error("File not found\n");
  }

  sudoku_board sb;

  std::string line;
  uint line_cnt = 0;
  while (std::getline(file, line)) {
    std::stringstream ss_line(line);
    std::string str_num;
    uint row = 0;
    while (std::getline(ss_line, str_num, ' ')) {

      sudoku_number sn = str_to_num(str_num);

      if (row == SUDOKU_BRD_SIZE)
        throw std::runtime_error("Row size is to large\n");

      if (line_cnt == SUDOKU_BRD_SIZE)
        throw std::runtime_error("Column size is to large\n");

      sb[line_cnt][row] = sn;
      row++;
    }
    if (row != SUDOKU_BRD_SIZE) {
      throw std::runtime_error("Row size is to small\n");
    }
    line_cnt++;
  }
  if (line_cnt != SUDOKU_BRD_SIZE) {
    throw std::runtime_error("Column size is to small\n");
  }

  return sb;
}

uint constexpr sqrt(uint const n) {
  if (n == 0)
    return 0;
  for (uint i = 1; i < n; i++) {
    if (i * i == n)
      return i;
  }
  return 0;
}

void print_board(sudoku_board const &sb) {
  constexpr uint sq_sz = sqrt(SUDOKU_BRD_SIZE);
  static_assert(sq_sz * sq_sz == SUDOKU_BRD_SIZE,
                "Board size must be root of integer");

  auto print_line = [](uint len) {
    for (uint i = 0; i < len; i++)
      std::cout << "-=-";
    std::cout << '\n';
  };

  for (uint i = 0; i < SUDOKU_BRD_SIZE; i++) {
    if (i % sq_sz == 0) {
      print_line(SUDOKU_BRD_SIZE-1);
    }
    for (uint j = 0; j < SUDOKU_BRD_SIZE+1; j++) {
      if (j % sq_sz == 0) {
        std::cout << "| ";
      }
      if (i < SUDOKU_BRD_SIZE && j < SUDOKU_BRD_SIZE) {
        if (char c = sb[i][j]) {
          std::cout << (char)(c + '0');
        } else {
          std::cout << (char)'X';
        }
        std::cout << ' ';
      }
    }
    std::cout << '\n';
  }
}