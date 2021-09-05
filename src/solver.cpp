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

auto gen_points(Sudoku_board const &bd) {
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

auto gen_all_cases(Sudoku_board const &sb) {
  std::array<std::array<std::array<sudoku_number, 10>, 3>, SUDOKU_BRD_SIZE> out;

  for (uint i = 0; i < sb.size; i++) {
    const numbers_rule row = gen_row(sb, {i, 0});
    const numbers_rule column = gen_column(sb, {0, i});
    const Point pt{(i / 3) * 3, (i % 3) * 3};
    const numbers_rule square = gen_square(sb, pt);

    std::array<sudoku_number, SUDOKU_BRD_SIZE + 1> r_h{0}, c_h{0}, w_h{0};
    for_each(row, [&r_h](uint i) { r_h[i]++; });
    for_each(column, [&c_h](uint i) { c_h[i]++; });
    for_each(square, [&w_h](uint i) { w_h[i]++; });

    out[i] = {r_h, c_h, w_h};
  }

  return out;
}

static_vector<Possible_number, SUDOKU_AREA_SIZE>
find_candidates(Sudoku_board const &bd) {
  static_vector<Possible_number, SUDOKU_AREA_SIZE> ps_out;

  // precompute lookup table
  const auto unsolved_points = gen_points(bd);
  const auto possible_values = gen_all_cases(bd);

  for (Point const &pt : unsolved_points) {
    // find missing number in row, column and window
    const uint py{pt.y}, px{pt.x}, pw{(pt.y / 3) * 3 + pt.x / 3};
    const auto r_v = possible_values[py][0];
    const auto c_v = possible_values[px][1];
    const auto w_v = possible_values[pw][2];

    // if number is can be this possition add it to list of caandidate moves
    static_vector<sudoku_number, SUDOKU_BRD_SIZE> candidates;
    for (uint i = 1; i < 10; i++) {
      const bool is_num_valid = !r_v[i] && !c_v[i] && !w_v[i];
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
    } else {
      return valid;
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