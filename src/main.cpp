#include "cnf_exp.h"
#include "bf_sat_solver.h"
#include "greedy_sat_solver.h"
#include <iostream>
#include <vector>
#include <string>

int main(int argc, char** argv) {
  // parameters
  char* path = (char*)"./test/test1.cnf";
  std::vector<bool> input = {true, false, true, true};
  // load the cnf expression
  CNF_exp cnf(path);
  // compute a partial evaluation
  CNF_exp part_eval = cnf.partial_eval(1);
  // evaluate the expression on the input
  bool output = cnf.eval(input);
  // run the exhaustive solver
  BF_SAT_Solver solver;
  int sat = solver.check(cnf);
  // run the greedy solver
  Greedy_SAT_Solver greedy_solver;
  int greedy_sat = solver.check(cnf);
  // print the CNF expression
  cnf.print();
  std::cout << std::endl;
  // print the partial evaluation
  part_eval.print();
  std::cout << "Input: ";
  for (unsigned i = 0; i < input.size(); ++i)
    std::cout << input[i] << ' ';
  std::cout << std::endl;
  std::cout << "Output: " << output << std::endl;
  // print whether the expression is satisfiable
  std::cout << "Satisfiable: " << sat << std::endl;
  std::cout << "Greedy SAT: " << greedy_sat << std::endl;
  return 0;
}
