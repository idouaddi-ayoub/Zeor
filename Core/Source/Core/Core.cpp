#include "Core.h"
#include <iostream>

namespace Core 
{

	void testFunc()
	{
		int x{ getUserValue() }, y{ getUserValue() };
		std::cout << Calc::multiple(x, y);
		std::cout << Calc::sum(x, y);
	}

	int getUserValue()
	{
		std::cout << "\nEnter int\n";

		int input{};
		std::cin >> input;

		return input;
	}

}

namespace Calc {

	int multiple(int x, int y)
	{
		std::cout << "\nMutliple: ";
		return x * y;
	}

	int sum(int x, int y)
	{
		std::cout << "\nSummation: ";
		return x + y;
	}

}