#include "pch.h"
#include "framework.h"
#include <stdexcept>
#include <functional>
//#include <iostream>
//#include <cmath>
//#include <clocale>

double calculateAreaSimpson(const std::function<double(double)>& f, double a, double b, int n)
{
    if (n <= 0 || n % 2 != 0)
    {
        throw std::invalid_argument("-");
    }

    double h = (b - a) / n;
    double sum = f(a) + f(b);

    for (int i = 1; i < n; i += 2)
    {
        sum += 4 * f(a + i * h);
    }

    for (int i = 2; i < n - 1; i += 2)
    {
        sum += 2 * f(a + i * h);
    }

    return sum * h / 3.0;
}

double targetFunction(double x)
{
    return 1.0;
}

/*
int main()
{
    setlocale(LC_ALL, "Russian");

    double a = 2.0;
    double b = 4.0;
    double exact_value = 2.0;

    int partitions[] = {2, 4, 10, 20, 100, 1000, 10000};
    int num_of_partitions = sizeof(partitions) / sizeof(partitions[0]);

    std::cout << "N / Площадь / Погрешность" << std::endl;

    for (int i = 0; i < num_of_partitions; ++i)
    {
        int n = partitions[i];
        double calculated_area = calculateAreaSimpson(targetFunction, a, b, n);
        double error = std::abs(calculated_area - exact_value);

        std::cout << n << " / " << calculated_area << " / " << error << std::endl;
    }

    std::cout << std::endl;

    system("pause");

    return 0;
}
*/