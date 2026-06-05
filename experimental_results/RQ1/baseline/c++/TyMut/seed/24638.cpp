
#include <boost/shared_ptr.hpp>

class base; // {};

class derived; // : public base {};

void func(base* p)
{
}

int main(int argc, char** argv)
{
	boost::shared_ptr<derived> p;
	func(p.get());

	return 0;
}
