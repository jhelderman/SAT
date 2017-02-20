#pragma once
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>


class CNF_exp {
  std::vector<std::vector<int> > clauses;
  unsigned num_literals;
  bool false_exp;
  const std::vector<std::vector<int> > FALSE_EXP = {{}};

 public:
  CNF_exp();
  CNF_exp(char* path);
  CNF_exp(
    const unsigned &num_literals,
    const std::vector<std::vector<int> > &clauses,
    const bool &false_exp=false);
  void load(char* path);
  bool eval(std::vector<bool> input);
  CNF_exp partial_eval(const int &var_id, const bool &val);
  void print();
  unsigned get_num_literals();
};
