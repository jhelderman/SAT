#include "sa_sat_solver.h"


SA_SAT_Solver::SA_SAT_Solver(double T0, double T_min, double cooling_factor) {
  this->T0 = T0;
  this->T_min = T_min;
  this->cooling_factor = cooling_factor;
}

int SA_SAT_Solver::check(CNF_exp exp) {
  std::vector<int> best_assignment;
  int best_sat;
  const unsigned max_tries = 1;
  return this->check(exp, max_tries, best_assignment, best_sat);
}


int SA_SAT_Solver::check(CNF_exp exp, std::vector<int> &best_assignment, int &best_sat) {
  const unsigned max_tries = 1;
  return this->check(exp, max_tries, best_assignment, best_sat);
}


void flip(std::vector<int> &current_assignment, std::unordered_set<int> &assignment_set, const unsigned &v) {
  assignment_set.erase(current_assignment[v]);
  current_assignment[v] *= -1;
  assignment_set.insert(current_assignment[v]);
}


void update_global_max(
    const std::vector<int> &current_assignment, const int &current_sat,
    std::vector<int> &best_assignment, int &best_sat) {
  if (current_sat > best_sat) {
    best_sat = current_sat;
    best_assignment = current_assignment;
  }
}


int SA_SAT_Solver::check(
    CNF_exp exp, const unsigned &max_tries, std::vector<int> &best_assignment,
    int &best_sat) {
  unsigned i = 0;
  unsigned v;
  double T, delta;
  unsigned num_variables = exp.get_num_literals();
  int num_clauses = (int)exp.get_num_clauses();
  std::vector<int> current_assignment(num_variables);
  std::unordered_set<int> assignment_set;
  int current_sat = 0;
  int new_sat = 0;
  // initialize the output variables
  best_sat = 0;
  // loop over the tries
  for (i = 0; i < max_tries; ++i) {
    // get a random initial assignment
    current_assignment = random_assignment(num_variables);
    assignment_set = vec2set(current_assignment);
    current_sat = satisfied_clauses(exp, assignment_set);
    // update global max
    update_global_max(current_assignment, current_sat, best_assignment, best_sat);
    if (best_sat == num_clauses)
      return 1;
    // initialize the temperature
    T = this->T0;
    // loop over the variable flips
    while (T > this->T_min) {
      // iterate over the variables
      for (v = 0; v < num_variables; ++v) {
        // temp flip
        flip(current_assignment, assignment_set, v);
        // compute the increase in the number of clauses satisfied
        new_sat = satisfied_clauses(exp, assignment_set);
        delta = (double)((int)new_sat - (int)current_sat);
        // check if the flip is accepted
        if (!(simulated_annealing_flip(delta, T))) {
          // revert flip if rejected
          flip(current_assignment, assignment_set, v);
        }
        // update the global max
        update_global_max(current_assignment, current_sat, best_assignment, best_sat);
        if (best_sat == num_clauses)
          return 1;
      }
    }
    // update the temperature
    T *= this->cooling_factor;
  }
  return 0;
}


int SA_SAT_Solver::check(
    CNF_exp exp, const unsigned &max_tries, const double &time_limit,
    std::vector<int> &best_assignment, int &best_sat) {
  unsigned i = 0;
  unsigned v;
  double T, delta;
  unsigned num_variables = exp.get_num_literals();
  int num_clauses = (int)exp.get_num_clauses();
  std::vector<int> current_assignment(num_variables);
  std::unordered_set<int> assignment_set;
  int current_sat = 0;
  int new_sat = 0;
  std::chrono::steady_clock::time_point start_time =
      std::chrono::steady_clock::now();
  // initialize the output variables
  best_sat = 0;
  // loop over the tries
  for (i = 0; i < max_tries; ++i) {
    // get a random initial assignment
    current_assignment = random_assignment(num_variables);
    assignment_set = vec2set(current_assignment);
    current_sat = satisfied_clauses(exp, assignment_set);
    // update global max
    update_global_max(current_assignment, current_sat, best_assignment, best_sat);
    if (best_sat == num_clauses)
      return 1;
    // initialize the temperature
    T = this->T0;
    // loop over the variable flips
    while (T > this->T_min) {
      // iterate over the variables
      for (v = 0; v < num_variables; ++v) {
        // check whether time limit exceeded
        if (get_time(start_time) < time_limit)
          return -1;
        // temp flip
        flip(current_assignment, assignment_set, v);
        // compute the increase in the number of clauses satisfied
        new_sat = satisfied_clauses(exp, assignment_set);
        delta = (double)((int)new_sat - (int)current_sat);
        // check if the flip is accepted
        if (!(simulated_annealing_flip(delta, T))) {
          // revert flip if rejected
          flip(current_assignment, assignment_set, v);
        }
        // update the global max
        update_global_max(current_assignment, current_sat, best_assignment, best_sat);
        if (best_sat == num_clauses)
          return 1;
      }
    }
    // update the temperature
    T *= this->cooling_factor;
  }
  return 0;
}
