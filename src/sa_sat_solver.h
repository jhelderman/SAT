#pragma once
#include "cnf_exp.h"
#include "sat_solver.h"
#include <vector>
#include "sat_utils.h"


class SA_SAT_Solver : public SAT_Solver {
  /*
    A SAT solver that uses the Simulated Annealing local search algorithm
  */
  double T0;
  double T_min;
  double cooling_factor;
 public:
  SA_SAT_Solver(double T0, double T_min, double cooling_factor);
  int check(CNF_exp exp);
  int check(CNF_exp exp, std::vector<int> &best_assignment, int &best_sat);
  int check(CNF_exp exp, const unsigned &max_tries, std::vector<int> &best_assignment, int &best_sat);
  int check(
      CNF_exp exp, const unsigned &max_tries, const double &time_limit,
      std::vector<int> &best_assignment, int &best_sat);
};
