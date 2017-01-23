#include "cnf_exp.h"
#include <iostream>
#include <vector>

int main(int argc, char** argv) {
  char* path = "/home/jhelderman/E/school/comb-opt/test.cnf";
  std::vector<bool> input = {true, true};
  CNF_exp cnf(path);
  bool output = cnf.eval(input);
  std::cout << "Input: ";
  for (unsigned i = 0; i < input.size(); ++i)
    std::cout << input[i] << ' ';
  std::cout << std::endl;
  std::cout << "Output: " << output << std::endl;
  return 0;
}
