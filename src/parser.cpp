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

Sudoku_board parse_input(std::string const &input_name) {
  std::ifstream file(input_name);

  if (!file.is_open()) {
    throw std::runtime_error("File not found\n");
  }

  Sudoku_board sb;

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

      sb({line_cnt, row}) = sn;
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

std::ostream &operator<<(std::ostream &os, Sudoku_board const &sb) {
  const uint sq_sz = std::sqrt(SUDOKU_BRD_SIZE);

  auto print_line = [&](uint len) {
    for (uint i = 0; i < len; i++)
      os << "-=-";
    os << '\n';
  };

  for (uint i = 0; i < sb.size; i++) {
    if (i % sq_sz == 0) {
      print_line(SUDOKU_BRD_SIZE - 1);
    }
    for (uint j = 0; j < sb.size + 1; j++) {
      if (j % sq_sz == 0) {
        os << "| ";
      }
      if (i < sb.size && j < sb.size) {
        if (char c = sb({i, j})) {
          os << (char)(c + '0');
        } else {
          os << (char)'X';
        }
        os << ' ';
      }
    }
    os << '\n';
  }
  return os;
}

uint Sudoku_board::num_of_unsolved() const {
  uint cnt = 0;
  for (auto const &l : board) {
    cnt += std::count_if(l.begin(), l.end(), [](auto &el) { return el == 0; });
  }
  return cnt;
}

bool is_row_valid(Sudoku_board const &bd, Point const &pt) {
  std::array<sudoku_number, SUDOKU_BRD_SIZE> w{0};
  const auto row = *(bd.begin() + pt.y);
  for (sudoku_number const n : row) {
    if (n == 0)
      continue;
    sudoku_number &hist = w[n - 1];
    if (hist == 1)
      return false;
    else
      hist++;
  }
  return true;
}

bool is_column_valid(Sudoku_board const &bd, Point const &pt) {
  std::array<sudoku_number, SUDOKU_BRD_SIZE> column;
  for (uint i = 0; i < SUDOKU_BRD_SIZE; i++) {
    column[i] = bd({i, pt.x});
  }

  std::array<sudoku_number, SUDOKU_BRD_SIZE> w{0};
  for (sudoku_number n : column) {
    if (n == 0)
      continue;
    sudoku_number &hist = w[n - 1];
    if (hist == 1)
      return false;
    else
      hist++;
  }
  return true;
}

bool is_window_valid(Sudoku_board const &bd, Point const &pt) {
  const uint s_w = sqrt(SUDOKU_BRD_SIZE);
  const Point s_pt(pt.quantize(s_w));

  // count number of each number in window, if same number appears twice then
  // board is invalid
  std::array<sudoku_number, SUDOKU_BRD_SIZE> w{0};
  for (uint i = 0; i < s_w; i++) {
    for (uint j = 0; j < s_w; j++) {
      const sudoku_number number = bd({i + s_pt.y, j + s_pt.x});
      if (number == 0)
        continue;
      sudoku_number &hist = w[number - 1];
      if (hist == 1)
        return false;
      else
        hist++;
    }
  }
  return true;
}

bool check_all_windows(Sudoku_board const &bd) {
  const uint sn = sqrt(SUDOKU_BRD_SIZE);
  for (uint i = 0; i < bd.size; i += sn) {
    for (uint j = 0; j < bd.size; j += sn) {
      if (!is_window_valid(bd, {i, j})) {
        // std::cout << "Invalid at window: " <<Point(i, j) << '\n';
        return false;
      }
    }
  }
  return true;
}

bool is_board_valid(Sudoku_board const &bd) {
  if (!check_all_windows(bd)) {
    // std::cout << "Invalid in window\n";
    return false;
  }

  for (uint i = 0; i < bd.size; i++) {
    for (uint j = 0; j < bd.size; j++) {
      const Point pt{i, j};
      const bool is_row_col_valid =
          is_column_valid(bd, pt) && is_row_valid(bd, pt);
      if (!is_row_col_valid) {
        // std::cout << "invalid here " << pt << '\n';
        return false;
      }
    }
  }
  return true;
}
