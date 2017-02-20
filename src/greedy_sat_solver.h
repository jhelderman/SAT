#pragma once
#include "cnf_exp.h"
#include <set>


class Greedy_SAT_Solver {
  void optimal_assignment(CNF_exp exp, CNF_exp &opt_exp, int &opt_literal);
 public:
  // constructors
  Greedy_SAT_Solver();

  // check method
  int check(CNF_exp exp);
  int check(CNF_exp exp, std::set<int> &partial_assignment);
};

