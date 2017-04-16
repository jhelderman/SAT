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


std::vector<int> random_assignment(unsigned num_literals) {
  // initialize the random number rng
  std::uniform_int_distribution<int> distribution(0, 1);
  // initialize the assignment
  std::vector<int> assignment(num_literals);
  // randomly generate an assignment
  for (unsigned i = 0; i < num_literals; ++i)
    assignment[i] = (2 * distribution(rng) - 1) * (i + 1);
  return assignment;
}
