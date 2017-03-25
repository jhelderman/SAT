#include "ilp_sat_solver.h"
#include <iostream>
#include <string>
#include <fstream>
#include "stdlib.h"


void CNF_to_AMPL(CNF_exp exp, const char* filename, const char* run_filename, int timelimit) {
  std::vector<std::vector<int> > clauses = exp.get_clauses();
  unsigned num_literals = exp.get_num_literals();
  // initialize the variables
  std::FILE* model_file;
  model_file = fopen(filename, "w");
  fprintf(model_file, "var x {i in 1..%u} binary;\n", num_literals);
  // write the objective function
  fprintf(model_file, "maximize objective: 0;");
  // iterate through the clauses and write the constraints
  unsigned clause_size = 0;
  std::vector<int> clause;
  for (unsigned i = 0; i < clauses.size(); ++i) {
    // iterate through the literals in the clause
    fprintf(model_file, "subject to C%u: ", i);
    clause_size = clauses[i].size();
    clause = clauses[i];
    for (unsigned j = 0; j < clause_size - 1; ++j) {
      // write the part of the constraint for the current variable
      if (clauses[i][j] > 0)
        fprintf(model_file, "x[%i] + ", clause[j]);
      else
        fprintf(model_file, "(1 - x[%i]) + ", -clause[j]);

    }
    // write the last part of the constraint
    if (clause[clause_size - 1] > 0)
      fprintf(model_file, "x[%i] >= 1;\n", clause[clause_size - 1]);
    else
      fprintf(model_file, "(1 - x[%i]) >= 1;\n", -clause[clause_size - 1]);
  }
  fclose(model_file);

  // write the AMPL script
  std::FILE* run_file;
  run_file = fopen(run_filename, "w");
  fprintf(run_file, "model %s;", filename);
  fprintf(run_file, "option solver cplex;");
  if (timelimit > 0)
    fprintf(run_file, "option cplex_options 'timelimit=%i';", timelimit);
  fprintf(run_file, "solve;");
  fprintf(run_file, "display x;");
  fclose(run_file);
}


ILP_SAT_Solver::ILP_SAT_Solver() {}


int ILP_SAT_Solver::check(CNF_exp exp) {
  std::string model_filename = "./temp.mod";
  std::string run_filename = "./temp.run";
  std::string output_filename = "./temp.out";
  return this->check(exp, model_filename, run_filename, output_filename);
}

int ILP_SAT_Solver::check(
        CNF_exp exp,
        std::string model_filename,
        std::string run_filename,
        std::string output_filename) {
  return this->check(exp, model_filename, run_filename, output_filename, -1);
}


int ILP_SAT_Solver::check(
        CNF_exp exp,
        std::string model_filename,
        std::string run_filename,
        std::string output_filename,
        int timelimit) {
  // write the model file and the AMPL script
  CNF_to_AMPL(exp, model_filename.c_str(), run_filename.c_str(), timelimit);
  // generate the command
  // len("ampl ") + len(" > ") + 1 = 9
  char* cmd = new char[9 + run_filename.size() + output_filename.size()];
  sprintf(cmd, "ampl %s > %s", run_filename.c_str(), output_filename.c_str());
  std::cout << cmd << std::endl;
  // std::system(cmd);
  return 0;
}
