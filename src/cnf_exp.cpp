#include "cnf_exp.h"
#include <iostream>


void split(const std::string &s, char delim, std::vector<std::string> &elems) {
  std::stringstream ss;
  ss.str(s);
  std::string item;
  while (std::getline(ss, item, delim)) {
    elems.push_back(item);
  }
}


std::vector<std::string> split(const std::string &s, char delim) {
  std::vector<std::string> elems;
  split(s, delim, elems);
  return elems;
}


CNF_exp::CNF_exp() {
  std::vector<std::vector<int> > clauses();
}


CNF_exp::CNF_exp(char* path) {
  std::vector<std::vector<int> > clauses();
  this->load(path);
}


void CNF_exp::load(char* path) {
  // initialize the file
  std::string line;
  std::ifstream fid(path);

  if (fid.is_open()) {
    // find the beginning of the logical expression
    std::getline(fid, line);
    while (line[0] != 'p') {
      std::getline(fid, line);
    }
    // parse the logical expression
    unsigned i;
    std::vector<std::string> split_line;
    std::getline(fid, line);
    while (line[0] != '%') {
      split_line = split(line, ' ');
      std::vector<int> clause(0);
      for (i = 0; i < split_line.size() - 1; ++i) {
        if (!split_line[i].empty())
          clause.push_back(std::atoi(split_line[i].c_str()));
      }
      this->clauses.push_back(clause);
      std::getline(fid, line);
    }
  }
}


bool CNF_exp::eval(std::vector<bool> input) {
  bool output = true;
  bool clause_val;
  unsigned i, j;
  // iterate through the clauses
  for (i = 0; i < this->clauses.size(); ++i){
    // compute the logical value of the clause
    clause_val = false;
    for (j = 0; j < this->clauses[i].size(); ++j) {
      if (this->clauses[i][j] > 0) {
        clause_val |= input[this->clauses[i][j] - 1];
      } else {
        clause_val |= !input[-this->clauses[i][j] - 1];
      }
    }
    // compute the conjunction of this clause with all previous clauses
    output &= clause_val;
  }
  return output;
}
