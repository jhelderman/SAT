#include "sat_utils.h"


std::default_random_engine rng;


double get_time(std::chrono::steady_clock::time_point start_time) {
  std::chrono::steady_clock::time_point now =
      std::chrono::steady_clock::now();
  std::chrono::duration<double> T =
      std::chrono::duration_cast<std::chrono::duration<double> >(now - start_time);
  return T.count();
}


void print(std::set<int> a) {
  for (std::set<int>::iterator it = a.begin(); it != a.end(); ++it)
    std::cout << *it << " ";
  std::cout << std::endl;
}


void print(std::vector<int> a) {
  for (std::vector<int>::iterator it = a.begin(); it != a.end(); ++it)
    std::cout << *it << " ";
  std::cout << std::endl;
}


std::unordered_set<int> vec2set(std::vector<int> assignment) {
  std::unordered_set<int> assignment_set;
  for (unsigned i = 0; i < assignment.size(); ++i)
    assignment_set.insert(assignment[i]);
  return assignment_set;
}


int satisfied_clauses(CNF_exp exp, const std::unordered_set<int> &assignment_set) {
  /*
    Determines the number of clauses satisfied by the given assignment

    inputs:
      exp: a logical expression in conjunctive normal form
      assignment: an assignment of the logical variables in the expression

    outputs:
      num_sat: the number of clauses satisfied by the assignment
   */
  unsigned i, j;
  // get the clauses from the expression
  std::vector<std::vector<int> > clauses = exp.get_clauses();
  // iterate through the clauses
  int num_sat = 0;
  bool satisfied = false;
  for (i = 0; i < clauses.size(); ++i) {
    // check if the clause is satisfied
    satisfied = false;
    for (j = 0; j < clauses[i].size(); ++j) {
      if (assignment_set.count(clauses[i][j]) != 0) {
        satisfied = true;
        break;
      }
    }
    // increment the counter if satisfied
    if (satisfied)
      ++num_sat;
  }
  return num_sat;
}


void unit_propagate(CNF_exp &exp, std::set<int> &partial_assignment) {
  std::vector<int> singletons = exp.singleton_clauses();
  while (singletons.size() > 0) {
    exp = exp.partial_eval(singletons);
    for (unsigned i = 0; i < singletons.size(); ++i) {
      partial_assignment.insert(singletons[i]);
    }
    singletons = exp.singleton_clauses();
  }
}


void optimal_assignment(CNF_exp exp, int &opt_literal) {
  CNF_exp opt_exp;
  optimal_assignment(exp, opt_exp, opt_literal);
}


void optimal_assignment(CNF_exp exp, CNF_exp &opt_exp, int &opt_literal) {
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


std::vector<int> random_assignment(unsigned num_literals) {
  // initialize the distribution
  std::uniform_int_distribution<int> distribution(0, 1);
  // initialize the assignment
  std::vector<int> assignment(num_literals);
  // randomly generate an assignment
  for (unsigned i = 0; i < num_literals; ++i)
    assignment[i] = (2 * distribution(rng) - 1) * (i + 1);
  return assignment;
}


std::vector<int> random_assignment(CNF_exp exp, std::set<int> &partial_assignment) {
  // initialize the distribution
  std::uniform_int_distribution<int> distribution(0, 1);
  // handle base cases and recur
  if (exp.get_variables().size() == 0) {
    std::vector<int> assignment;
    for (std::set<int>::iterator it = partial_assignment.begin(); it != partial_assignment.end(); ++it)
      assignment.push_back(*it);
    return assignment;
  } else {
    int literal = (2 * distribution(rng) - 1) * (*exp.get_variables().begin());
    partial_assignment.insert(literal);
    return random_assignment(exp.partial_eval(literal), partial_assignment);
  }
}


std::vector<int> greedy_assignment(CNF_exp exp) {
  std::set<int> partial_assignment;
  return greedy_assignment(exp, partial_assignment);
}


std::vector<int> greedy_assignment(CNF_exp exp, std::set<int> &partial_assignment) {
  CNF_exp opt_exp;
  int opt_literal;
  // chose the optimal literal to assign
  optimal_assignment(exp, opt_exp, opt_literal);
  // get the optimal partial evaluation and assignment
  partial_assignment.insert(opt_literal);
  opt_exp = exp.partial_eval(opt_literal);
  unit_propagate(opt_exp, partial_assignment);
  // handle base cases and recur
  if (opt_exp.is_true_exp() || opt_exp.is_false_exp())
    // randomly assign the rest of the literals
    // in the case of a true expression, these correspond to "don't care"
    // literals
    // in the case of a false expression, we are just arbitrarily assigning
    // literals so that we have a full assignment
    return random_assignment(opt_exp, partial_assignment);
  else
    // continue to assign literals
    return greedy_assignment(opt_exp, partial_assignment);
}
