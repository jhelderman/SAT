#include "gsat_solver.h"
#include <unordered_set>


int best_neighbor(CNF_exp exp, std::vector<int> assignment, int &best_sat) {
  /*
    Computes the best neighbor given an expression and a starting assignment

    parameters
      exp: the logical expression in conjunctive normal form
      assignment: a starting assignment for which the neighborhood will be
        calculated
      best_sat: an output parameter that is the number of clauses satisfied by
        the best neighbor

   outputs:
     best_neighbor: the neighbor that satisfies the most clauses encoded as the
       index of the assignment variable to flip.
   */
  // initialize loop variables to track the best assignment flip
  int best_flip = -1;
  best_sat = 0;
  // initialize buffer variables
  int num_sat = 0;
  std::vector<int> temp_assignment(assignment);
  std::unordered_set<int> assignment_set = vec2set(assignment);
  // iterate through the neighborhood
  for (unsigned i = 0; i < temp_assignment.size(); ++i) {
    // flip the assignment at position i
    assignment_set.erase(temp_assignment[i]);
    temp_assignment[i] *= -1;
    assignment_set.insert(temp_assignment[i]);
    // check the number of clauses satisfied
    num_sat = satisfied_clauses(exp, assignment_set);
    // flip the assignment back to the original value
    assignment_set.erase(temp_assignment[i]);
    temp_assignment[i] *= -1;
    assignment_set.insert(temp_assignment[i]);
    // update the best flip
    if (num_sat > best_sat) {
      best_sat = num_sat;
      best_flip = i;
    }
  }
  // return the best neighbor
  return best_flip;
}


// std::vector<int> greedy_assignment(CNF_exp exp) {
//   std::vector<int> assignment;
//   return assignment;
// }


void GSAT_Solver::GSAT_update(
    CNF_exp exp, std::vector<int> &current_assignment, int &current_sat,
    bool &done) {
  // check whether the expression is satisfied
  if ((unsigned)current_sat == exp.get_num_clauses()) {
    done = true;
    return;
  }
  // find the best neighbor
  int best_flip = best_neighbor(exp, current_assignment, current_sat);
  current_assignment[best_flip] *= -1;
}


GSAT_Solver::GSAT_Solver() {}


std::vector<int> get_initial_assignment(CNF_exp exp, const int &init_method) {
  /*
    Returns the initial assignment, given the initialization method code

    init_method: The initialization method code. The mapping is as follows.
      1 (default): random assignment
      2: greedy assignment
   */
  std::vector<int> initial_assignment;
  switch (init_method) {
    case 1:
      initial_assignment = random_assignment(exp.get_num_literals());
    case 2:
      initial_assignment = greedy_assignment(exp);
    default:
      initial_assignment = random_assignment(exp.get_num_literals());
  }
  return initial_assignment;
}


int GSAT_Solver::check(CNF_exp exp) {
  // use random by default
  std::vector<int> initial_assignment = get_initial_assignment(exp, 1);
  std::vector<int> best_assignment;
  int best_sat = 0;
  return this->check(exp, initial_assignment, best_sat);
}


int GSAT_Solver::check(CNF_exp exp, const int &init_method, std::vector<int> &best_assignment, int &best_sat) {
  best_assignment = get_initial_assignment(exp, init_method);
  return this->check(exp, best_assignment, best_sat);
}


int GSAT_Solver::check(CNF_exp exp, const int &init_method, const double &time_limit, std::vector<int> &best_assignment, int &best_sat) {
  best_assignment = get_initial_assignment(exp, init_method);
  return this->check(exp, best_assignment, time_limit, best_sat);
}


int GSAT_Solver::check(
    CNF_exp exp, std::vector<int> &initial_assignment, int &best_sat) {
  // initialize loop variables
  bool done = false;
  std::unordered_set<int> assignment_set = vec2set(initial_assignment);
  std::vector<int> current_assignment(initial_assignment);
  best_sat = satisfied_clauses(exp, assignment_set);
  int current_sat = best_sat;
  // iteratively perform the local search procedure
  while (!done) {
    this->GSAT_update(exp, current_assignment, current_sat, done);
    if (current_sat > best_sat) {
      best_sat = current_sat;
      initial_assignment = current_assignment;
    }
  }
  // return the result
  if ((unsigned)best_sat == exp.get_num_clauses()) {
    // satisfiable
    return 1;
  } else {
    // no satisfying assignment found
    return 0;
  }
  return 0;
}


int GSAT_Solver::check(
    CNF_exp exp, std::vector<int> &initial_assignment, const double &time_limit, int &best_sat) {
  // initialize loop variables
  bool done = false;
  std::unordered_set<int> assignment_set = vec2set(initial_assignment);
  std::vector<int> current_assignment(initial_assignment);
  best_sat = satisfied_clauses(exp, assignment_set);
  int current_sat = best_sat;
  std::chrono::steady_clock::time_point start_time =
      std::chrono::steady_clock::now();
  // iteratively perform the local search procedure
  while (!done && get_time(start_time) < time_limit) {
    // run the GSAT iteration
    this->GSAT_update(exp, current_assignment, current_sat, done);
    // update the global maximum
    if (current_sat > best_sat) {
      best_sat = current_sat;
      initial_assignment = current_assignment;
    }
  }
  // return the result
  if (get_time(start_time) >= time_limit)
    // ran out of time
    return -1;
  else if ((unsigned)best_sat == exp.get_num_clauses())
    // satisfiable
    return 1;
  else
    // no satisfying assignment found
    return 0;
}


int GSAT_Solver::check(
    CNF_exp exp, const unsigned max_flips, const unsigned &max_tries,
    std::vector<int> &best_assignment, int &best_sat) {
  best_sat = 0;
  // iterate through the possible tries
  for (unsigned i = 0; i < max_tries; ++i) {
    // initialize loop variables
    std::vector<int> initial_assignment = random_assignment(exp.get_num_literals());
    std::vector<int> current_assignment(initial_assignment);
    std::unordered_set<int> assignment_set = vec2set(initial_assignment);
    int current_sat = satisfied_clauses(exp, assignment_set);
    bool done = false;
    // iteratively perform the local search procedure
    for (unsigned j = 0; j < max_flips; ++j) {
      // do the local search
      this->GSAT_update(exp, current_assignment, current_sat, done);
      // update the global best assignments
      if (current_sat > best_sat) {
        best_sat = current_sat;
        best_assignment = current_assignment;
      }
      // check if an assignment or local minimum was found
      if (done && (unsigned)best_sat == exp.get_num_clauses())
        return 1;
    }
  }
  // no satisfying assignment found
  return 0;
}
