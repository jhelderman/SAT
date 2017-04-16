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
    CNF_exp exp, std::vector<int> &current_assignment, int &best_sat,
    bool &done) {
  // check whether the expression is satisfied
  if ((unsigned)best_sat == exp.get_num_clauses()) {
    done = true;
    return;
  }
  // find the best neighbor
  int temp_sat = 0;
  int best_flip = best_neighbor(exp, current_assignment, temp_sat);
  // compare to the current assignment
  if (temp_sat >= best_sat) {
    current_assignment[best_flip] *= -1;
    best_sat = temp_sat;
  } else {
    done = true;
  }
}


GSAT_Solver::GSAT_Solver() {}


int GSAT_Solver::check(CNF_exp exp) {
  std::vector<int> initial_assignment = random_assignment(exp.get_num_literals());
  return this->check(exp, initial_assignment);
}


int GSAT_Solver::check(CNF_exp exp, const double &time_limit) {
  std::vector<int> initial_assignment = random_assignment(exp.get_num_literals());
  return this->check(exp, initial_assignment, time_limit);
}


int GSAT_Solver::check(
    CNF_exp exp, std::vector<int> &initial_assignment) {
  // initialize loop variables
  bool done = false;
  std::unordered_set<int> assignment_set = vec2set(initial_assignment);
  int best_sat = satisfied_clauses(exp, assignment_set);
  // iteratively perform the local search procedure
  while (!done) {
    this->GSAT_update(exp, initial_assignment, best_sat, done);
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
    CNF_exp exp, std::vector<int> &initial_assignment, const double &time_limit) {
  // initialize loop variables
  bool done = false;
  std::unordered_set<int> assignment_set = vec2set(initial_assignment);
  int best_sat = satisfied_clauses(exp, assignment_set);
  std::chrono::steady_clock::time_point start_time =
      std::chrono::steady_clock::now();
  // iteratively perform the local search procedure
  while (!done && get_time(start_time) < time_limit) {
    this->GSAT_update(exp, initial_assignment, best_sat, done);
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
    print(initial_assignment);
    std::unordered_set<int> assignment_set = vec2set(initial_assignment);
    int try_best_sat = satisfied_clauses(exp, assignment_set);
    bool done = false;
    // iteratively perform the local search procedure
    for (unsigned j = 0; j < max_flips; ++j) {
      // do the local search
      this->GSAT_update(exp, initial_assignment, try_best_sat, done);
      // update the global best assignments
      if (try_best_sat > best_sat) {
        best_sat = try_best_sat;
        best_assignment = initial_assignment;
      }
      // check if an assignment or local minimum was found
      if (done && (unsigned)try_best_sat == exp.get_num_clauses())
        return 1;
      else if (done)
        break;
    }
  }
  // no satisfying assignment found
  return 0;
}
