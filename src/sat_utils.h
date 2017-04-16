#pragma once
#include <chrono>
#include <set>
#include <vector>
#include <iostream>
#include <unordered_set>
#include "cnf_exp.h"
#include <random>

double get_time(std::chrono::steady_clock::time_point start_time);
void print(std::set<int> a);
void print(std::vector<int> a);
std::unordered_set<int> vec2set(std::vector<int> assignment);
int satisfied_clauses(CNF_exp exp, const std::unordered_set<int> &assignment_set);
std::vector<int> random_assignment(unsigned num_literals);
