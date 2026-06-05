
#include <boost/spirit/include/classic.hpp>
#include <boost/spirit/include/phoenix1.hpp>

#include <stdio.h>

int main()
{
	int variable = 123;
	
	using namespace boost::spirit::classic;
	using namespace phoenix;

	rule<> testRule
		// =  eps_p [ var( variable ) = 101101 ] //that one works fine with -O2 -fno-strict-aliasing
		=  eps_p [ assign_a( variable, 101101 ) ] //this one gives rubbish with -O2 -fno-strict-aliasing
		;	
	
	parse_info<> result = parse( "bla bla", testRule );
	
	if ( result.hit )
		printf( "%d\n", variable );
	
	return 0;
}
