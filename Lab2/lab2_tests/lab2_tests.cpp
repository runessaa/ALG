#include "pch.h"
#include "CppUnitTest.h"
#include "C:\Users\nastya runessaa\source\repos\lab2\framework.h"
#include <cmath>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace AreaCalculatorTests
{
	TEST_CLASS(SimpsonRuleTests)
	{
	public:
		TEST_METHOD(TestForGivenFunctions_CosX_plus2_and_CosX_plus1)
		{
			double a = 2.0;
			double b = 4.0;
			int n = 100;
			double expected_area = 2.0;
			double tolerance = 1e-12;

			double calculated_area = calculateAreaSimpson(targetFunction, a, b, n);

			Assert::AreEqual(expected_area, calculated_area, tolerance, L"-");
		}

		TEST_METHOD(TestSimpsonRule_ForSin_Integral_0_to_PI)
		{
			auto sin_func = [](double x) { return sin(x); };
			double a = 0.0;
			double b = 3.14159265358979323846;
			int n = 1000;
			double expected_area = 2.0;
			double tolerance = 1e-6;

			double calculated_area = calculateAreaSimpson(sin_func, a, b, n);

			Assert::AreEqual(expected_area, calculated_area, tolerance, L"-");
		}

		TEST_METHOD(TestForInvalid_N_ThrowsException)
		{
			auto func = [](double x) { return x; };

			auto action_odd_n = [&]() {
				calculateAreaSimpson(func, 0, 1, 3);
				};

			auto action_zero_n = [&]() {
				calculateAreaSimpson(func, 0, 1, 0);
				};

			Assert::ExpectException<std::invalid_argument>(action_odd_n, L"-");
			Assert::ExpectException<std::invalid_argument>(action_zero_n, L"-");
		}
	};
}