#include "cnf_exp.h"
#include "bf_sat_solver.h"
#include <iostream>
#include <vector>

int main(int argc, char** argv) {
  char* path = "./test/test1.cnf";
  std::vector<bool> input = {true, false, true, true};
  CNF_exp cnf(path);
  CNF_exp part_eval = cnf.partial_eval(1, false);
  BF_SAT_Solver solver;
  cnf.print();
  std::cout << std::endl;
  part_eval.print();
  bool output = cnf.eval(input);
  std::cout << "Input: ";
  for (unsigned i = 0; i < input.size(); ++i)
    std::cout << input[i] << ' ';
  std::cout << std::endl;
  std::cout << "Output: " << output << std::endl;
  // run the exhaustive solver
  int sat = solver.check(cnf);
  std::cout << "Satisfiable: " << sat << std::endl;
  return 0;
}
