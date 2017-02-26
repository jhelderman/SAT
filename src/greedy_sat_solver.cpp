#include "greedy_sat_solver.h"
#include <iostream>


Greedy_SAT_Solver::Greedy_SAT_Solver() {}


void print_set(std::set<int> s) {
  for (std::set<int>::iterator it = s.begin(); it != s.end(); ++it)
    std::cout << *it << " ";
  std::cout << std::endl;
}

void print_vec(std::vector<int> a) {
  for (unsigned i = 0; i < a.size(); ++i)
    std::cout << a[i] << " ";
  std::cout << std::endl;
}


int Greedy_SAT_Solver::check(CNF_exp exp) {
  std::set<int> partial_assignment;
  return this->check(exp, partial_assignment);
}


int Greedy_SAT_Solver::check(CNF_exp exp, std::set<int> &partial_assignment) {
  // check the satisfiability of a CNF expression by a greedy strategy. This
  // strategy will find the assignment that maximally reduces the length of
  // the CNF expression at each step. However, this procedure will not, in
  // general, determine the satisfiability of the expression. If the algorithm
  // returns a satisfying assignment, the answer will be correct. However, if
  // the algorithm returns UNSAT, it is possible that a satisfying assignment
  // exists, but was not found.
  // determine the optimal partial assignment
  CNF_exp opt_exp;
  int opt_literal;
  this->optimal_assignment(exp, opt_exp, opt_literal);
  partial_assignment.insert(opt_literal);
  opt_exp = exp.partial_eval(opt_literal);
  this->unit_propagate(opt_exp, partial_assignment);
  // reduce the size of the problem and recur
  if (opt_exp.is_true_exp())
    return 1;
  else if (opt_exp.is_false_exp())
    return 0;
  else
    return this->check(opt_exp, partial_assignment);
}
