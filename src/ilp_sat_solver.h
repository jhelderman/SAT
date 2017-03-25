#pragma once
#include "cnf_exp.h"
#include "sat_solver.h"
#include <string>


class ILP_SAT_Solver : public SAT_Solver {
 public:
  ILP_SAT_Solver();
  int check(CNF_exp exp);
  int check(
        CNF_exp exp,
        std::string model_filename,
        std::string run_filename,
        std::string output_filename);
  int check(
        CNF_exp exp,
        std::string model_filename,
        std::string run_filename,
        std::string output_filename,
        int timelimit);
};
