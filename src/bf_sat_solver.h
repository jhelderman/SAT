#pragma once
#include "cnf_exp.h"
#include <vector>


class BF_SAT_Solver {
 public:
  BF_SAT_Solver();
  bool check(CNF_exp exp);
};
