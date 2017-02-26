#pragma once
#include "cnf_exp.h"
#include "sat_solver.h"
#include <set>


class Greedy_SAT_Solver : public SAT_Solver {
 public:
  // constructors
  Greedy_SAT_Solver();

  // check method
  int check(CNF_exp exp);
  int check(CNF_exp exp, std::set<int> &partial_assignment);
};

