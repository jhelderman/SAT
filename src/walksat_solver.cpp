#include "walksat_solver.h"


std::default_random_engine generator;


WALKSAT_Solver::WALKSAT_Solver(double p) {
  this->p = p;
}


int WALKSAT_Solver::check(CNF_exp exp, const unsigned max_flips, const unsigned &max_tries, std::vector<int> &best_assignment, int &best_sat) {
  best_sat = 0;
  // iterate through the possible tries
  for (unsigned i = 0; i < max_tries; ++i) {
    // initialize loop variables
    std::vector<int> initial_assignment = random_assignment(exp.get_num_literals());
    std::unordered_set<int> assignment_set = vec2set(initial_assignment);
    int try_best_sat = satisfied_clauses(exp, assignment_set);
    int temp_sat = try_best_sat;
    std::vector<int> temp_assignment(initial_assignment);
    bool done = false;
    // iteratively perform the local search procedure
    for (unsigned j = 0; j < max_flips; ++j) {
      // do the local search
      this->WALKSAT_update(exp, temp_assignment, temp_sat, done);
      // update the global best assignments
      if (temp_sat > best_sat) {
        best_sat = temp_sat;
        best_assignment = temp_assignment;
      }
      // check if an assignment or local minimum was found
      if (done && (unsigned)try_best_sat == exp.get_num_clauses())
        return 1;
    }
  }
  // no satisfying assignment found
  return 0;
}


void WALKSAT_Solver::WALKSAT_update(CNF_exp exp, std::vector<int> &current_assignment, int &num_sat, bool &done) {
  // check whether the expression is satisfied
  if ((unsigned)num_sat == exp.get_num_clauses()) {
    done = true;
    return;
  }
  // draw to see if we go on a random walk
  std::uniform_real_distribution<double> distribution(0.0,1.0);
  std::uniform_int_distribution<unsigned> idx_distribution(0, current_assignment.size()-1);
  bool random_walk = distribution(generator) < this->p;
  // update the state variables
  if (random_walk) {
    // randomly change a bit
    unsigned random_idx = idx_distribution(generator);
    current_assignment[random_idx] *= -1;
  } else {
    // do the standard GSAT update
    this->GSAT_update(exp, current_assignment, num_sat, done);
    // don't finish if we haven't found a satisfying assignment
    // this exists because GSAT_update will finish if it finds a local max
    if (done && !((unsigned)num_sat == exp.get_num_clauses()))
      done = false;
  }

}
