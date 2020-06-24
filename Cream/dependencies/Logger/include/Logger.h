#include <iostream>
#include <string>

namespace Logger
{
	static void Print(std::string& string)
	{
		std::cout << string << "\n";
	}
}