
#include <iostream>
#include <limits>

// Assumption: If a is a signed variable, then a % -1 should always be 0
//
// ok    if myType is of type char or short
// fails if myType is of type int  or long

int main() {

	typedef int myType;

	myType c;

	myType a = std::numeric_limits<myType>::min();
	int    b = -1;

        // always ok
	c = a % -1;
	std::cout << a << ", " << b << ", " << c << std::endl;

        // signals EXC ARITHMETIC:Arithmetic exception 
        // if myType is int or is long 
	c = a % b;
        std:: cout << a << ", " << b << ", " << c << std::endl;

	return 0;
}
