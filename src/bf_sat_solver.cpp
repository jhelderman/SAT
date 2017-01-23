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


bool BF_SAT_Solver::check(CNF_exp exp) {
  // initialize loop variables
  unsigned num_literals = exp.get_num_literals();
  std::vector<bool> input(num_literals, false);
  // iterate over the inputs
  while (!all(input)) {
    // check this input
    if (exp.eval(input))
      return true;
    // increment the input
    inc_input(input);
  }
  if (exp.eval(input))
    return true;
  else
    return false;
}
