#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

#include "calc_parser.h"

template <class T>
void Check(T&& expression, double check_value)
{
	try
	{
		auto result = expression_parse(std::forward<T>(expression));
		BOOST_CHECK(result == check_value);
	}
	catch (const std::exception&)
	{
		BOOST_CHECK(false);
	}
}

BOOST_AUTO_TEST_CASE(simple_2_add_2)
{
	Check("2+2",4.0);
}

BOOST_AUTO_TEST_CASE(simple_expression)
{
	Check("-1 + 5 - 3", 1.0);
}

BOOST_AUTO_TEST_CASE(mult_and_dev_test)
{
	Check("-10 + (8 * 2.5) - (3 / 1,5)", 8.0);
}

BOOST_AUTO_TEST_CASE(diff_expression)
{
	Check("1 + (2 * (2.5 + 2.5 + (3 - 2))) - (3 / 1.5)", 11.0);
}

BOOST_AUTO_TEST_CASE(error_test)
{
	try
	{
		auto result = expression_parse("1.1 + 2.1 + abc");
		BOOST_CHECK(false); //never get here
	}
	catch (const std::exception& e)
	{
		BOOST_CHECK(std::string(e.what()) == "Incorrect input: + abc");
	}
}


