#include "dpll_sat_solver.h"
#include <iostream>


DPLL_SAT_Solver::DPLL_SAT_Solver() {}


int DPLL_SAT_Solver::check(CNF_exp exp) {
  std::set<int> partial_assignment;
  return this->check(exp, partial_assignment);
}


int DPLL_SAT_Solver::check(CNF_exp exp, std::set<int> &partial_assignment) {
  // remove all singleton clauses
  this->unit_propagate(exp, partial_assignment);
  // handle base cases
  if (exp.is_false_exp()) {
    return 0;
  } else if (exp.is_true_exp()) {
    return 1;
  }
  // select a variable
  // std::cout << "Select Variable" << std::endl;
  int literal;
  this->optimal_assignment(exp, literal);
  // recur
  // std::cout << "First Check" << std::endl;
  std::set<int> temp_partial_assignment(partial_assignment);
  temp_partial_assignment.insert(literal);
  int sat = this->check(exp.partial_eval(literal), temp_partial_assignment);
  if (sat == 1) {
    // std::cout << "SAT" << std::endl;
    partial_assignment = temp_partial_assignment;
    // std::cout << "Return" << std::endl;
    return sat;
  } else {
    // std::cout << "Second Check" << std::endl;
    partial_assignment.insert(-literal);
    return this->check(exp.partial_eval(-literal), partial_assignment);
  }
}
