#include "solver.hpp"

numbers_rule gen_row(Sudoku_board const &bd, Point const &pt) {
  return *(bd.begin() + pt.y);
}

numbers_rule gen_column(Sudoku_board const &bd, Point const &pt) {
  numbers_rule nb;
  for (uint i = 0; i < bd.size; i++) {
    nb[i] = bd({i, pt.x});
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

bool insert_numbers(Sudoku_board &sb, std::vector<Possible_number> const &pn) {
  bool valid = false;
  for (Possible_number const &p : pn) {
    if (p.candidate.size() == 1) {
      sb({p.pt.y, p.pt.x}) = p.candidate[0];
      valid = true;
    }
  }
  return valid;
}

bool brute_fore_search(Sudoku_board &sb) {
  std::vector<Possible_number> pn;
  for (Node const &nn : gen_nodes(sb)) {
    pn.push_back(find_candidates(nn));
  }
  std::sort(pn.begin(), pn.end());

  for (Possible_number &best : pn) {
    Sudoku_board virtual_board = sb;

    for (sudoku_number const n : best.candidate) {
      virtual_board(best.pt) = n;

      single_solve(virtual_board);
      if (!is_board_valid(virtual_board)) {
        auto iter = std::find(best.candidate.begin(), best.candidate.end(), n);
        best.candidate.erase(iter);
      }
    }

    if (best.candidate.size() == 1) {
      sb(best.pt) = best.candidate[0];
    }
    if (sb.num_of_unsolved() == 0)
      return true;
    single_solve(sb);
  }
  return false;
}

void single_solve(Sudoku_board &sb) {
  while (true) {
    std::vector<Node> n = gen_nodes(sb);
    std::vector<Possible_number> pn;
    for (Node const &nn : n) {
      pn.push_back(find_candidates(nn));
    }
    std::sort(pn.begin(), pn.end());
    if (!insert_numbers(sb, pn))
      break;
  }
}