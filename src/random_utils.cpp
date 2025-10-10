#include "random_utils.h"
#include <cmath>
#include <cstdlib>

double powerab(double a, int b) {
    return (b == 0) ? 1.0 : (b < 0 ? 1.0 / powerab(a, -b) : a * powerab(a, b - 1));
}

int factor(int a) {
    return (a == 0) ? 1 : a * factor(a-1);
}

double expn(double n) {
    double result = 0;
    for (int i = 0; i <= 17; i++) {
        result += powerab(n, i) / double(factor(i));
    }
    return result;
}

double RandomUniform(double min, double max) {
    double random = static_cast<double>(std::rand()) / static_cast<double>(RAND_MAX);
    return min + random * (max - min);
}

double RandomBernoulli(double p) {
    return RandomUniform(0.0, 1.0) < p ? 1 : 0;
}

double RandomPoisson(double lambda) {
    double L = expn(-lambda);
    int k = 0;
    double p = 1.0;
    do {
        k++;
        p *= RandomUniform(0.0, 1.0);
    } while (p > L);
    return k-1;
}

double RandomNormal(double mean, double stddev) {
    double pi = 3.14159265358979323846;
    double u1 = RandomUniform(0.0, 1.0);
    double u2 = RandomUniform(0.0, 1.0);
    double z0 = sqrt(-2.0 * log(u1)) * cos(2.0 * pi * u2);
    return z0 * stddev + mean;
}
