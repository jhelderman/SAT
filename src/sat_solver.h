#pragma once
#include "cnf_exp.h"
#include <vector>
#include <set>


class SAT_Solver {
 protected:
  void optimal_assignment(CNF_exp exp, int &opt_literal);
  void optimal_assignment(CNF_exp exp, CNF_exp &opt_exp, int &opt_literal);
  void unit_propagate(CNF_exp &exp, std::set<int> &partial_assignment);
 public:
  SAT_Solver();
  virtual int check(CNF_exp exp) = 0;
};
