#pragma once
#include "cnf_exp.h"
#include "sat_solver.h"
#include <set>


class DPLL_SAT_Solver : public SAT_Solver {
 public:
  DPLL_SAT_Solver();
  int check(CNF_exp exp);
  int check(CNF_exp exp, std::set<int> &partial_assignment);
};
