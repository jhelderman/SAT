#include "bf_sat_solver.h"
#include <iostream>


bool all(const std::vector<bool> &x) {
  bool output = true;
  for (unsigned i = 0; i < x.size(); ++i)
    output &= x[i];
  return output;
}


void inc_input(std::vector<bool> &x) {
  bool carry = true;
  for (unsigned i = 0; i < x.size(); ++i) {
    bool new_digit = x[i] ^ carry;
    carry = x[i] & carry;
    x[i] = new_digit;
  }
}


BF_SAT_Solver::BF_SAT_Solver() {}


int BF_SAT_Solver::check(CNF_exp exp) {
  // initialize loop variables
  unsigned num_literals = exp.get_num_literals();
  std::vector<bool> input(num_literals, false);
  // iterate over the inputs
  while (!all(input)) {
    // check this input
    if (exp.eval(input))
      return 1;
    // increment the input
    inc_input(input);
  }
  if (exp.eval(input))
    return 1;
  else
    return 0;
}


// double get_time(std::chrono::steady_clock::time_point start_time) {
//   std::chrono::steady_clock::time_point now =
//     std::chrono::steady_clock::now();
//   std::chrono::duration<double> T =
//     std::chrono::duration_cast<std::chrono::duration<double> >(now - start_time);
//   return T.count();
// }


int BF_SAT_Solver::check(CNF_exp exp, const double &time_limit) {
  // initialize loop variables
  unsigned num_literals = exp.get_num_literals();
  std::vector<bool> input(num_literals, false);
  std::chrono::steady_clock::time_point start_time =
    std::chrono::steady_clock::now();
  // iterate over the inputs
  while (!all(input) && get_time(start_time) < time_limit) {
    // check this input
    if (exp.eval(input))
      return 1;
    // increment the input
    inc_input(input);
  }
  if (get_time(start_time) >= time_limit)
    return -1;
  else if (exp.eval(input))
    return 1;
  else
    return 0;
}
