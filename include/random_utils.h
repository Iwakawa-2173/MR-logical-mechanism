#ifndef RANDOM_UTILS_H
#define RANDOM_UTILS_H

double powerab(double a, int b);
int factor(int a);
double expn(double n);

double RandomUniform(double min, double max);
double RandomBernoulli(double p);
double RandomPoisson(double lambda);
double RandomNormal(double mean, double stddev);

#endif // RANDOM_UTILS_H
