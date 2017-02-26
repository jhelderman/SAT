#include "sat_solver.h"
#include <iostream>


SAT_Solver::SAT_Solver() {}


void SAT_Solver::unit_propagate(CNF_exp &exp, std::set<int> &partial_assignment) {
  std::vector<int> singletons = exp.singleton_clauses();
  while (singletons.size() > 0) {
    exp = exp.partial_eval(singletons);
    for (unsigned i = 0; i < singletons.size(); ++i) {
      partial_assignment.insert(singletons[i]);
    }
    singletons = exp.singleton_clauses();
  }
}


void SAT_Solver::optimal_assignment(CNF_exp exp, int &opt_literal) {
  CNF_exp opt_exp;
  this->optimal_assignment(exp, opt_exp, opt_literal);
}


void SAT_Solver::optimal_assignment(CNF_exp exp, CNF_exp &opt_exp, int &opt_literal) {
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
    this->unit_propagate(buf, empty_set);
    buf_len = buf.get_length();
    // update the optimal assignment
    if (buf_len < opt_len && !buf.is_false_exp()) {
      opt_exp = buf;
      opt_literal = *it;
      opt_len = opt_exp.get_length();
    }
  }
}
