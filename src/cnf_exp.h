#include <vector>
#include <string>
#include <fstream>
#include <sstream>


class CNF_exp {
  std::vector<std::vector<int> > clauses;

 public:
  CNF_exp();
  CNF_exp(char* path);
  void load(char* path);
  bool eval(std::vector<bool> input);
};
