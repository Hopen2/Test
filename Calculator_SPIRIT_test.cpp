// Calculator_SPIRIT_test.cpp : Defines the entry point for the console application.
//

#include <iostream>
#include <iomanip>
#include "calc_parser.h"



int main()
{
	std::string expression;
	
	std::cout << "Enter expression or press Enter to exit" << std::endl;

	while (true)
	{
		std::getline(std::cin, expression);
		if (expression.empty())
			break;

		try
		{
			auto result = expression_parse(expression);
			std::cout << expression << " == " << std::setprecision(2) << result << std::endl;
		}
		catch (const std::exception& e)
		{
			std::cout << e.what() << std::endl;
		}
	}
	
    return 0;
}

