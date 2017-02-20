#include "cnf_exp.h"


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


std::vector<std::string> filter_empty(const std::vector<std::string> &v) {
  std::vector<std::string> output;
  for (unsigned i = 0; i < v.size(); ++i) {
    if (!v.at(i).empty()) {
      output.push_back(v.at(i));
    }
  }
  return output;
}


CNF_exp::CNF_exp() {
  std::vector<std::vector<int> > clauses();
  this->num_literals = 0;
  this->false_exp = false;
}


CNF_exp::CNF_exp(char* path) {
  std::vector<std::vector<int> > clauses();
  this->num_literals = 0;
  this->false_exp = false;
  this->load(path);
}


CNF_exp::CNF_exp(
    const unsigned &num_literals,
    const std::vector<std::vector<int> > &clauses,
    const bool &false_exp) {
  this->num_literals = num_literals;
  this->clauses = clauses;
  this->false_exp = false_exp;
}


unsigned CNF_exp::get_num_literals() {
  return this->num_literals;
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
    std::vector<std::string> header = split(line, ' ');
    header = filter_empty(header);
    this->num_literals = std::atoi(header[2].c_str());
    unsigned num_clauses = std::atoi(header[3].c_str());
    // parse the logical expression
    unsigned i, j;
    std::vector<std::string> split_line;
    for (j = 0; j < num_clauses; ++j) {
      // split the line into its constituent literals
      std::getline(fid, line);
      split_line = split(line, ' ');
      split_line = filter_empty(split_line);
      // add the literals to the clause
      std::vector<int> clause(0);
      for (i = 0; i < split_line.size() - 1; ++i) {
        if (!split_line[i].empty())
          clause.push_back(std::atoi(split_line[i].c_str()));
      }
      // add the clause the list
      this->clauses.push_back(clause);
    }
  }
}


bool CNF_exp::eval(std::vector<bool> input) {
  // check for a false expression
  if (this->false_exp)
    return false;
  // evaluate the expression
  bool output = true;
  bool clause_val;
  unsigned i, j;
  // iterate through the clauses
  for (i = 0; i < this->clauses.size(); ++i){
    // compute the logical value of the clause
    clause_val = false;
    for (j = 0; j < this->clauses[i].size(); ++j) {
      if (this->clauses.at(i).at(j) > 0) {
        clause_val |= input.at(this->clauses.at(i).at(j) - 1);
      } else {
        clause_val |= !input.at(-this->clauses.at(i).at(j) - 1);
      }
    }
    // compute the conjunction of this clause with all previous clauses
    output &= clause_val;
  }
  return output;
}


void CNF_exp::print() {
  // check for a false expression
  if (this->false_exp) {
    std::cout << "false" << std::endl;
    return;
  }
  // print out the expression
  unsigned i, j;
  for (i = 0; i < this->clauses.size(); ++i) {
    for (j = 0; j < this->clauses[i].size(); ++j)
      std::cout << this->clauses[i][j] << ' ';
    std::cout << std::endl;
  }
}



CNF_exp CNF_exp::partial_eval(const int &var_id, const bool &val) {
  // iterate through the clauses
  bool clause_true;
  bool clause_false;
  int clause_var_id;
  std::vector<std::vector<int> > clauses;
  for (unsigned i = 0; i < this->clauses.size(); ++i) {
    // check for the variable in the given clause
    clause_true = false;
    clause_false = false;
    std::vector<int> clause;
    for (unsigned j = 0; j < this->clauses.at(i).size(); ++j) {
      clause_var_id = this->clauses.at(i).at(j);
      if (  // clause is true
          (clause_var_id == var_id && val) ||
          (clause_var_id == -var_id && !val)) {
        clause_true = true;
        break;
      } else if (  // literal does not match, remove from clause
          (clause_var_id == var_id && !val) ||
          (clause_var_id == -var_id && val)) {
        // do nothing
        clause_false = true;
        break;
      } else {  // variable does not match, no effect on clause
        clause.push_back(clause_var_id);
      }
    }
    // add the modified clause
    if (clause_true) {
      // do nothing
    } else if (clause_false && clause.size() == 0) {
      // clause is false. this means the expression is false
      return CNF_exp(this->num_literals, FALSE_EXP, true);
    } else if (!clause_true && clause.size() != 0) {
      clauses.push_back(clause);
    }
  }
  // construct a new CNF expression from the result
  return CNF_exp(this->num_literals, clauses);
}
