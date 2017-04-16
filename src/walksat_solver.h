#pragma once
#include "gsat_solver.h"
#include <random>


class WALKSAT_Solver : public GSAT_Solver {
  /*
    A SAT solver that uses the WALKSAT local search algorithm
  */
  double p; // the probability of random walk
 public:
  WALKSAT_Solver(double p);
  int check(CNF_exp exp, const unsigned max_flips, const unsigned &max_tries, std::vector<int> &best_assignment, int &best_sat);
  void WALKSAT_update(CNF_exp exp, std::vector<int> &current_assignment, int &best_sat, bool &done);
};
