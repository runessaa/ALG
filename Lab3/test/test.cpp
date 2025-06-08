#include "pch.h"
#include "CppUnitTest.h"
#include "C:\Users\nastya runessaa\source\repos\algorithms\Header.h"
#include <vector>
#include <algorithm>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace SortsTest
{
	TEST_CLASS(SortsTests)
	{

	public:

		TEST_METHOD(ShellSortTest)
		{
			std::vector<int> arr = {12, 34, 54, 2, 3};
			std::vector<int> sortedArr = {2, 3, 12, 34, 54};
			shellSort(arr);

			for (size_t i = 0; i < arr.size(); ++i)
			{
				Assert::AreEqual(sortedArr[i], arr[i]);
			}
		}

		TEST_METHOD(InsertionSortTest)
		{
			std::vector<int> arr = {64, 34, 25, 12, 22, 11, 90};
			std::vector<int> sortedArr = {11, 12, 22, 25, 34, 64, 90};
			insertionSort(arr);

			Assert::AreEqual(sortedArr.size(), arr.size());

			for (size_t i = 0; i < arr.size(); ++i)
			{
				Assert::AreEqual(sortedArr[i], arr[i]);
			}
		}

		TEST_METHOD(QuickSortTest)
		{
			std::vector<int> arr = {10, 7, 8, 9, 1, 5};
			std::vector<int> sortedArr = {1, 5, 7, 8, 9, 10};
			quickSort(arr);

			for (size_t i = 0; i < arr.size(); ++i)
			{
				Assert::AreEqual(sortedArr[i], arr[i]);
			}
		}

		TEST_METHOD(AlreadySortedTest)
		{
			std::vector<int> arr = {1, 2, 3, 4, 5, 6};
			std::vector<int> arr_copy_shell = arr;
			std::vector<int> arr_copy_insertion = arr;
			std::vector<int> arr_copy_quick = arr;

			shellSort(arr_copy_shell);
			insertionSort(arr_copy_insertion);
			quickSort(arr_copy_quick);

			Assert::IsTrue(std::is_sorted(arr_copy_shell.begin(), arr_copy_shell.end()));
			Assert::IsTrue(std::is_sorted(arr_copy_insertion.begin(), arr_copy_insertion.end()));
			Assert::IsTrue(std::is_sorted(arr_copy_quick.begin(), arr_copy_quick.end()));
		}

		TEST_METHOD(EmptyArrayTest)
		{
			std::vector<int> arr_shell;
			std::vector<int> arr_insertion;
			std::vector<int> arr_quick;

			shellSort(arr_shell);
			insertionSort(arr_insertion);
			quickSort(arr_quick);

			Assert::IsTrue(arr_shell.empty());
			Assert::IsTrue(arr_insertion.empty());
			Assert::IsTrue(arr_quick.empty());
		}
	};
}