#pragma once
#include <vector>
#include <set>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>


const std::vector<std::vector<int> > FALSE_EXP = {};


class CNF_exp {
  // member variables
  std::vector<std::vector<int> > clauses;
  std::set<int> variables;
  unsigned num_literals;
  bool false_exp;

 public:
  // constructors
  CNF_exp();
  CNF_exp(char* path);
  CNF_exp(
    const unsigned &num_literals,
    const std::vector<std::vector<int> > &clauses,
    const bool &false_exp=false);
  CNF_exp(
    const unsigned &num_literals,
    const std::vector<std::vector<int> > &clauses,
    const std::set<int> variables,
    const bool &false_exp=false);

  // evaluation
  bool eval(std::vector<bool> input);
  CNF_exp partial_eval(const int &literal);

  // IO
  void load(char* path);
  void print();

  // instance information queries
  unsigned get_num_literals();
  unsigned get_length();
  bool is_false_exp();
  bool is_true_exp();
  std::set<int> get_variables();
};
