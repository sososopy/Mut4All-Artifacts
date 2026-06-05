#include <boost/spirit/include/karma.hpp>
#include <boost/spirit/include/support_utree.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/fusion/include/std_pair.hpp>
#include <iostream>
#include <string>

namespace karma = boost::spirit::karma;
using boost::spirit::karma::_1;
using boost::spirit::ascii::string;
using boost::phoenix::val;

template <typename G>
void test_generator(char const* expected, G const& g) {
    std::string s;
    std::back_insert_iterator<std::string> out(s);
    if (boost::spirit::karma::generate(out, g) && s == expected)
        std::cout << "ok" << std::endl;
    else
        std::cout << "fail" << std::endl;
}

int main() {
	test_generator("abc", karma::lazy(val(string))[_1 = "abc"]);

}
