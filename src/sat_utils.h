#pragma once
#include <chrono>
#include <set>
#include <vector>
#include <iostream>
#include <unordered_set>
#include "cnf_exp.h"
#include <random>
#include <math.h>

double get_time(std::chrono::steady_clock::time_point start_time);
void print(std::set<int> a);
void print(std::vector<int> a);
std::unordered_set<int> vec2set(std::vector<int> assignment);
int satisfied_clauses(CNF_exp exp, const std::unordered_set<int> &assignment_set);
void unit_propagate(CNF_exp &exp, std::set<int> &partial_assignment);
void optimal_assignment(CNF_exp exp, int &opt_literal);
void optimal_assignment(CNF_exp exp, CNF_exp &opt_exp, int &opt_literal);
std::vector<int> random_assignment(unsigned num_literals);
std::vector<int> random_assignment(CNF_exp exp, std::set<int> &partial_assignment);
std::vector<int> greedy_assignment(CNF_exp exp);
std::vector<int> greedy_assignment(CNF_exp exp, std::set<int> &partial_assignment);
bool simulated_annealing_flip(double delta, double T);
