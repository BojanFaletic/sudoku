#pragma once
#include "parser.hpp"

class ScoreBoard {
public:
  ScoreBoard() : correct{0}, total{0} {}
  ScoreBoard(uint c, uint t) : correct{c}, total{t} {}
  uint correct, total;

  friend std::ostream &operator<<(std::ostream &os, ScoreBoard const &sb) {
    os << "Accuracy: " << sb.correct * 100 / sb.total << "%";
    return os;
  }
};