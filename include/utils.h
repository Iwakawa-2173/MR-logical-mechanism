#ifndef UTILS_H
#define UTILS_H

#include <vector>
#include <iostream>
#include <string>

double CalcM(std::vector<double> v);
int test_function(double input, double f1, double f2, double f3, int conf1, int conf2, double o1, double o2, double o3, bool print);

std::vector<std::vector<double>> generateAllBinaryVectors(int count_of_blocks);

std::ostream& operator<<(std::ostream& os, const std::vector<double>& v);

int binaryToDecimal(std::vector<double> digits);

void not_included_nums(int count_of_blocks, std::string notation);

void print_all_states(int count_of_blocks);

std::vector<double> multiplyMatrixByVector(std::vector<std::vector<double>> matrix, std::vector<double> vec);

#endif // UTILS_H
