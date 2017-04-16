#pragma once
#include "cnf_exp.h"
#include "sat_solver.h"
#include <vector>
#include "sat_utils.h"
#include <random>


class GSAT_Solver : public SAT_Solver {
  /*
    A SAT solver that uses the GSAT local search algorithm
   */
 public:
  GSAT_Solver();
  int check(CNF_exp exp);
  int check(CNF_exp exp, const double &time_limit);
  int check(CNF_exp exp, std::vector<int> &initial_assignment);
  int check(CNF_exp exp, std::vector<int> &initial_assignment, const double &time_limit);
  virtual int check(CNF_exp exp, const unsigned max_flips, const unsigned &max_tries, std::vector<int> &best_assignment, int &best_sat);
  void GSAT_update(CNF_exp exp, std::vector<int> &current_assignment, int &best_sat, bool &done);
};
