#include "solver.hpp"

numbers_rule gen_row(Sudoku_board const &bd, Point const &pt) {
  return *(bd.begin() + pt.y);
}

numbers_rule gen_column(Sudoku_board const &bd, Point const &pt) {
  numbers_rule nb;
  for (uint i = 0; i < bd.size; i++) {
    nb[i] = bd(Point(i, pt.x));
  }
  return nb;
}

numbers_rule gen_square(Sudoku_board const &bd, Point const &pt) {
  const uint sq_sz = std::sqrt(SUDOKU_BRD_SIZE);
  const Point s_pt{pt.quantize(sq_sz)};
  numbers_rule nb;
  uint idx = 0;
  for (uint i = 0; i < sq_sz; i++) {
    for (uint j = 0; j < sq_sz; j++) {
      nb[idx++] = bd({i + s_pt.y, j + s_pt.x});
    }
  }
  return nb;
}

std::vector<Node> gen_nodes(Sudoku_board const &bd) {
  std::vector<Node> nds;
  for (uint i = 0; i < bd.size; i++) {
    for (uint j = 0; j < bd.size; j++) {
      const Point pt{i, j};
      if (bd(pt) == 0) {
        Node nd{pt, gen_row(bd, pt), gen_column(bd, pt), gen_square(bd, pt)};
        nds.push_back(nd);
      }
    }
  }
  return nds;
}

std::vector<Possible_number> find_candidates(Sudoku_board const &bd) {
  std::vector<Possible_number> ps_out;
  for (Node const &nd : gen_nodes(bd)) {
    // find missing number in row, column and window
    std::vector<sudoku_number> candidates;
    candidates.reserve(9);
    for (uint i = 1; i < 10; i++) {
      const bool is_num_valid =
          any_of(nd.row, i) || any_of(nd.column, i) || any_of(nd.window, i);

      if (!is_num_valid)
        candidates.push_back(i);
    }
    ps_out.push_back(Possible_number{nd.pt, candidates});
  }
  std::sort(ps_out.begin(), ps_out.end());
  return ps_out;
}

bool insert_numbers(Sudoku_board &sb, std::vector<Possible_number> const &pn) {
  bool valid = false;
  for (Possible_number const &p : pn) {
    if (p.candidate.size() == 1) {
      sb(p.pt) = p.candidate.front();
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
      sb(best.pt) = best.candidate.front();

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