#pragma once
#include "cnf_exp.h"
#include <vector>
#include <chrono>


class BF_SAT_Solver {
 public:
  BF_SAT_Solver();
  int check(CNF_exp exp);
  int check(CNF_exp exp, const double &time_limit);
};
