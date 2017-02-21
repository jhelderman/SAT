#include "greedy_sat_solver.h"
#include <iostream>


Greedy_SAT_Solver::Greedy_SAT_Solver() {}


void print_set(std::set<int> s) {
  for (std::set<int>::iterator it = s.begin(); it != s.end(); ++it)
    std::cout << *it << " ";
  std::cout << std::endl;
}


void unit_propagate(CNF_exp &exp, std::set<int> &partial_assignment) {
  std::vector<int> singletons = exp.singleton_clauses();
  // bool disp = singletons.size() > 0;
  // if (disp)
  //   std::cout << "Singletons: ";
  while (singletons.size() > 0) {
    exp = exp.partial_eval(singletons);
    for (unsigned i = 0; i < singletons.size(); ++i) {
      // std::cout << singletons[i] << " ";
      partial_assignment.insert(singletons[i]);
    }
    singletons = exp.singleton_clauses();
  }
  // if (disp) {
  //   std::cout << std::endl;
  //   std::cout << std::endl;
  // }
}


void Greedy_SAT_Solver::optimal_assignment(CNF_exp exp, CNF_exp &opt_exp, int &opt_literal) {
  // looks for an optimal assignment among the variables in a CNF expression
  // the assignment is optimal in the sense that the resulting expression will
  // have the minimum number of clauses. There could possibly be multiple
  // assignments with this number of clauses
  // initialize function state
  std::set<int> variables = exp.get_variables();
  unsigned buf_len;
  CNF_exp buf;
  // print_set(variables);
  // initialize the optimal state
  opt_literal = *variables.begin();
  opt_exp = exp.partial_eval(opt_literal);
  unsigned opt_len = opt_exp.get_length();
  // iterate through the possible assignments
  for (std::set<int>::iterator it = variables.begin(); it != variables.end(); ++it) {
    // determine the length of the current assignment
    buf = exp.partial_eval(*it);
    std::set<int> empty_set;
    unit_propagate(buf, empty_set);
    buf_len = buf.get_length();
    // update the optimal assignment
    if (buf_len < opt_len && !buf.is_false_exp()) {
      opt_exp = buf;
      opt_literal = *it;
      opt_len = opt_exp.get_length();
    }
  }
}


int Greedy_SAT_Solver::check(CNF_exp exp) {
  std::set<int> partial_assignment;
  // std::cout << "Expression:" << std::endl;
  // exp.print();
  // std::cout << std::endl;
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
  // std::cout << "End Optimal Assignment Calculation" << std::endl;
  unit_propagate(opt_exp, partial_assignment);
  // std::cout << "Assignment: " << opt_literal << std::endl;
  // std::cout << "Resulting Expression:" << std::endl;
  // opt_exp.print();
  // std::cout << std::endl;
  // reduce the size of the problem and recur
  if (opt_exp.is_true_exp())
    return 1;
  else if (opt_exp.is_false_exp())
    return 0;
  else
    return this->check(opt_exp, partial_assignment);
}
