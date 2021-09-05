#include "solver.hpp"

numbers_rule gen_row(Sudoku_board const &bd, Point const &pt) {
  return *(bd.begin() + pt.y);
}

numbers_rule gen_column(Sudoku_board const &bd, Point const &pt) {
  numbers_rule nb;
  for (uint i = 0; i < bd.size; i++) {
    const Point pp(i, pt.x);
    nb[i] = bd(pp);
  }
  return nb;
}

numbers_rule gen_square(Sudoku_board const &bd, Point const &pt) {
  const uint sq_sz = std::sqrt(SUDOKU_BRD_SIZE);
  const Point s_pt{pt.quantize(sq_sz)};
  numbers_rule nb{0};
  uint idx = 0;
  for (uint i = s_pt.y; i < s_pt.y + sq_sz; i++) {
    for (uint j = s_pt.x; j < s_pt.x + sq_sz; j++) {
      const Point pp{i, j};
      nb[idx++] = bd(pp);
    }
  }
  return nb;
}

static_vector<Point, SUDOKU_AREA_SIZE> gen_points(Sudoku_board const &bd) {
  static_vector<Point, SUDOKU_AREA_SIZE> nds;
  for (uint i = 0; i < bd.size; i++) {
    for (uint j = 0; j < bd.size; j++) {
      const Point pt(i, j);
      if (bd(pt) == 0) {
        nds.push_back(pt);
      }
    }
  }
  return nds;
}

static_vector<Possible_number, SUDOKU_AREA_SIZE>
find_candidates(Sudoku_board const &bd) {
  static_vector<Possible_number, SUDOKU_AREA_SIZE> ps_out;

  for (Point const &pt : gen_points(bd)) {
    // find missing number in row, column and window

    // find number relevent numbers
    const numbers_rule row = gen_row(bd, pt);
    const numbers_rule column = gen_column(bd, pt);
    const numbers_rule window = gen_square(bd, pt);

    // calculate histogram
    std::array<sudoku_number, SUDOKU_BRD_SIZE + 1> r{0}, c{0}, w{0};
    for_each(row, [&r](uint i) { r[i]++; });
    for_each(column, [&c](uint i) { c[i]++; });
    for_each(window, [&w](uint i) { w[i]++; });

    // if number is can be this possition add it to list of caandidate moves
    static_vector<sudoku_number, SUDOKU_BRD_SIZE> candidates;
    for (uint i = 1; i < 10; i++) {
      const bool is_num_valid = !r[i] && !c[i] && !w[i];
      if (is_num_valid)
        candidates.push_back(i);
    }
    ps_out.push_back(Possible_number{pt, candidates});
  }
  std::sort(ps_out.begin(), ps_out.end());
  return ps_out;
}

bool insert_numbers(Sudoku_board &sb, auto const &pn) {
  bool valid = false;
  if (sb.is_solved())
    return false;
  for (Possible_number const &p : pn) {
    if (p.candidate.size() == 1) {
      sb(p.pt) = p.candidate[0];
      valid = true;
    }
  }
  return valid;
}

bool brute_fore_search(Sudoku_board &sb) {
  single_solve(sb);
  for (Possible_number &best : find_candidates(sb)) {
    Sudoku_board virtual_board = sb;
    auto remove_condition = [&](uint n) {
      virtual_board(best.pt) = n;
      single_solve(virtual_board);
      return !is_board_valid(virtual_board);
    };

    std::remove_if(best.candidate.begin(), best.candidate.end(),
                   remove_condition);

    if (best.candidate.size() == 1)
      sb(best.pt) = best.candidate[0];

    if (sb.is_solved())
      return true;
    else
      single_solve(sb);
  }
  return false;
}

void single_solve(Sudoku_board &sb) {
  while (true) {
    const auto pn = find_candidates(sb);
    if (!insert_numbers(sb, pn))
      break;
  }
}