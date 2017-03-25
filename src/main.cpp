#include "cnf_exp.h"
#include "bf_sat_solver.h"
#include "greedy_sat_solver.h"
#include "dpll_sat_solver.h"
#include "ilp_sat_solver.h"
#include <iostream>
#include <vector>
#include <string>
#include <cstring>


void print(std::set<int> a) {
  for (std::set<int>::iterator it = a.begin(); it != a.end(); ++it)
    std::cout << *it << " ";
  std::cout << std::endl;
}


int main(int argc, char** argv) {
  // An example of how to use the ILP SAT solver

  // parameters
  char* path = (char*)"./test/test1.cnf";
  char* mod_filename = "./test/test1.mod";
  char* run_filename = "./test/test1.run";
  char* output_filename = "./test/test1.out";
  int timelimit = -1;

  // load the CNF file
  CNF_exp exp(path);
  ILP_SAT_Solver solver;
  solver.check(exp, mod_filename, run_filename, output_filename, timelimit);
  return 0;
}

// int main(int argc, char** argv) {
//   // parameters
//   // char* path = (char*)"./test/test1.cnf";
//   // char* path = (char*)"/home/jhelderman/E/school/comb-opt/SAT-examples/uf20-91/uf20-02.cnf";
//   char* path = argv[1];
//   std::vector<bool> input = {true, false, true, true};
//   // load the cnf expression
//   std::cout << "Loading CNF Expression..." << std::endl;
//   CNF_exp cnf(path);
//   // compute a partial evaluation
//   std::cout << "Computing Partial Evaluation..." << std::endl;
//   CNF_exp part_eval = cnf.partial_eval(1);
//   // // evaluate the expression on the input
//   // std::cout << "Evaluating Expression..." << std::endl;
//   // bool output = cnf.eval(input);
//   // // run the exhaustive solver
//   // std::cout << "Running Exhaustive Solver..." << std::endl;
//   // BF_SAT_Solver solver;
//   // int sat = solver.check(cnf);
//   // run the greedy solver
//   std::cout << "Running Greedy Solver..." << std::endl;
//   Greedy_SAT_Solver greedy_solver;
//   int greedy_sat = greedy_solver.check(cnf);
//   // print the CNF expression
//   std::cout << "CNF Expression:" << std::endl;
//   cnf.print();
//   std::cout << std::endl;
//   // // print the partial evaluation
//   // std::cout << "Partial Evaluation:" << std::endl;
//   // part_eval.print();
//   // // print the full evaluation
//   // std::cout << "Input: ";
//   // for (unsigned i = 0; i < input.size(); ++i)
//   //   std::cout << input[i] << ' ';
//   // std::cout << std::endl;
//   // std::cout << "Output: " << output << std::endl;
//   // print whether the expression is satisfiable
//   // std::cout << "Satisfiable: " << sat << std::endl;
//   std::cout << "Greedy SAT: " << greedy_sat << std::endl;
//   return 0;
// }

// int main(int argc, char** argv) {
//   unsigned counter = 0;
//   unsigned nonsense_counter = 0;
//   const std::string nvar = "20";
//   const std::string nclause = "91";
//   for (unsigned i = 1; i < 101; ++i) {
//     // parameters
//     // char* path = (char*)"./test/test1.cnf";
//     std::string buf = ("/home/jhelderman/E/school/comb-opt/SAT-examples/uf" + nvar + "-" + nclause + "/uf" + nvar + "-0" + std::to_string(i) +  ".cnf");
//     char* path = new char[buf.length() + 1];
//     std::strcpy(path, buf.c_str());
//     // load the cnf expression
//     std::cout << "Loading CNF Expression #" << i << "..." << std::endl;
//     CNF_exp cnf(path);
//     // Greedy_SAT_Solver greedy_solver;
//     // std::set<int> assignment;
//     // int sat = greedy_solver.check(cnf, assignment);
//     DPLL_SAT_Solver dpll_solver;
//     std::set<int> assignment;
//     int sat = dpll_solver.check(cnf, assignment);
//     if (sat == 0) {
//       std::cout << "WRONG" << std::endl;
//       ++counter;
//     } else {
//       bool cnf_eval = cnf.eval(assignment);
//       if (!cnf_eval) {
//         std::cout << "Inconsistent Answer" << std::endl;
//         ++nonsense_counter;
//         print(assignment);
//       }
//     }
//     delete[] path;
//   }
//   std::cout << "Number of Variables: " << nvar << std::endl;
//   std::cout << "Number of Clauses: " << nclause << std::endl;
//   std::cout << counter << " / 100 Errors" << std::endl;
//   std::cout << nonsense_counter << " / 100 Illogical Answers" << std::endl;
//   return 0;
// }
