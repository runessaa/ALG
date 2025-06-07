#include "pch.h"
#include "CppUnitTest.h"
#include <unordered_map>
#include <string>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace DictionaryUnitTest
{
	TEST_CLASS(DictionaryEraseTest)
	{
	public:

		TEST_METHOD(TestElementDeletion)
		{
			std::unordered_map<int, std::string> my_map;
			my_map[10] = "ten";
			my_map[20] = "twenty";
			my_map[30] = "thirty";

			size_t elements_erased = my_map.erase(20);

			Assert::AreEqual(size_t{ 1 }, elements_erased);

			Assert::AreEqual(size_t{ 2 }, my_map.size());

			Assert::IsTrue(my_map.find(20) == my_map.end());

			Assert::IsFalse(my_map.find(10) == my_map.end());
			Assert::AreEqual(std::string("thirty"), my_map[30]);
		}
	};
}