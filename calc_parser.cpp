#include "calc_parser.h"

#  pragma warning(disable:4348)
#include <boost/config/warning_disable.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>

constexpr double my_round(double value)
{
	return static_cast<double>(static_cast<int>(value * 100 + ((value > 0) ? 0.5 : -0.5))) / 100;
}


namespace calc_parser
{
	template <typename Iterator>
	double parse(Iterator first, Iterator last)
	{
		boost::spirit::qi::rule<Iterator, double, boost::spirit::ascii::space_type> expr, term, factor, num, irr_num;

		num =
			boost::spirit::qi::char_("1234567890")[boost::spirit::qi::_val = boost::spirit::qi::_1 - 48];

		irr_num =
			boost::spirit::qi::uint_[boost::spirit::qi::_val = boost::spirit::qi::_1]
			>> ','
			>> num[boost::spirit::qi::_val += 0.1 * boost::spirit::qi::_1]
			>> -num[boost::spirit::qi::_val += 0.01 * boost::spirit::qi::_1];

		expr =
			term[boost::spirit::qi::_val = boost::spirit::qi::_1]
			>> *('+' >> term[boost::spirit::qi::_val += boost::spirit::qi::_1]
				| '-' >> term[boost::spirit::qi::_val -= boost::spirit::qi::_1]
				)
			;

		term =
			factor[boost::spirit::qi::_val = boost::spirit::qi::_1]
			>> *('*' >> factor[boost::spirit::qi::_val *= boost::spirit::qi::_1]
				| '/' >> factor[boost::spirit::qi::_val /= boost::spirit::qi::_1]
				)
			;

		factor =
			irr_num[boost::spirit::qi::_val = boost::spirit::qi::_1]
			| boost::spirit::qi::double_[boost::spirit::qi::_val = boost::spirit::qi::_1]
			| boost::spirit::qi::uint_[boost::spirit::qi::_val = boost::spirit::qi::_1]
			| '(' >> expr[boost::spirit::qi::_val = boost::spirit::qi::_1] >> ')'
			| '-' >> factor[boost::spirit::qi::_val = -boost::spirit::qi::_1]
			| '+' >> factor[boost::spirit::qi::_val = boost::spirit::qi::_1]
			;


		bool r = false;

		double result = 0;

		try
		{
			r = boost::spirit::qi::phrase_parse(
				first,
				last,
				(
					expr
					),
				boost::spirit::ascii::space,
				result
				);
		}
		catch (const boost::spirit::qi::expectation_failure<Iterator>& e)
		{
			throw (std::runtime_error("Incorrect input: " + std::string(e.first, e.last)));
		}
		catch (const std::exception& e)
		{
			throw (std::runtime_error(std::string("Incorrect input: ") + e.what()));
		}
		catch (...)
		{
			throw (std::runtime_error("Incorrect input: Unknown exception"));
		}

		if (first != last) // fail if we did not get a full match
		{
			throw (std::runtime_error("Incorrect input: " + std::string(first, last)));
		}

		return result;
	}

}

double expression_parse(std::string source)
{
	return my_round(calc_parser::parse(source.begin(), source.end()));
}