#pragma once
#include <functional>
double calculateAreaSimpson(const std::function<double(double)>& f, double a, double b, int n);
double targetFunction(double x);