#include "solver.hpp"

numbers_rule gen_row(sudoku_board const &bd, Point const &pt) {
  return bd[pt.y];
}

numbers_rule gen_column(sudoku_board const &bd, Point const &pt) {
  numbers_rule nb;
  for (uint i = 0; i < bd.size(); i++) {
    nb[i] = bd[i][pt.x];
  }
  return nb;
}

numbers_rule gen_square(sudoku_board const &bd, Point const &pt) {
  const uint sq_sz = std::sqrt(SUDOKU_BRD_SIZE);
  const Point s_pt{(pt.y / sq_sz) * sq_sz, (pt.x / sq_sz) * sq_sz};
  numbers_rule nb;
  uint idx = 0;
  for (uint i = 0; i < sq_sz; i++) {
    for (uint j = 0; j < sq_sz; j++) {
      nb[idx++] = bd[i + s_pt.y][j + s_pt.x];
    }
  }
  return nb;
}

std::vector<Node> gen_nodes(sudoku_board const &bd) {
  std::vector<Node> nds;
  for (uint i = 0; i < bd.size(); i++) {
    for (uint j = 0; j < bd.size(); j++) {
      if (bd[i][j] == 0) {
        const Point pt{i, j};
        Node nd{pt, gen_row(bd, pt), gen_column(bd, pt), gen_square(bd, pt)};
        nds.push_back(nd);
      }
    }
  }
  return nds;
}

Possible_number find_candidates(Node const &nd) {
  // find missing number in row, column and window
  std::vector<sudoku_number> c, r, s;
  for (uint i = 1; i < 10; i++) {
    if (!any_of(nd.row, i)) {
      r.push_back(i);
    }
    if (!any_of(nd.column, i)) {
      c.push_back(i);
    }
    if (!any_of(nd.window, i)) {
      s.push_back(i);
    }
  }

  // Candidate number is intersection within c, r, s
  std::vector<sudoku_number> candidates;
  for (sudoku_number n : r) {
    if (any_of(c, n) && any_of(r, n)) {
      candidates.push_back(n);
    }
  }

  Possible_number pn{nd.pt, candidates};
  return pn;
}

bool insert_numbers(sudoku_board &sb, std::vector<Possible_number> const &pn) {
  bool valid = false;
  for (Possible_number const &p : pn){
    if (p.candidate.size() == 1){
      sb[p.pt.y][p.pt.x] = p.candidate[0];
      valid = true;
    }
  }
  return valid;
}