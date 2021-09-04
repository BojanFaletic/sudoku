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

void print_board(sudoku_board const &sb) {
  const uint sq_sz = std::sqrt(SUDOKU_BRD_SIZE);

  auto print_line = [](uint len) {
    for (uint i = 0; i < len; i++)
      std::cout << "-=-";
    std::cout << '\n';
  };

  for (uint i = 0; i < sb.size(); i++) {
    if (i % sq_sz == 0) {
      print_line(SUDOKU_BRD_SIZE - 1);
    }
    for (uint j = 0; j < sb.size() + 1; j++) {
      if (j % sq_sz == 0) {
        std::cout << "| ";
      }
      if (i < sb.size() && j < sb.size()) {
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

uint num_of_unsolved(sudoku_board const &sb){
  uint cnt = 0;
  for (auto const &l : sb){
    cnt += std::count_if(l.begin(), l.end(), [](auto &el){return el == 0;});
  }
  return cnt;
}