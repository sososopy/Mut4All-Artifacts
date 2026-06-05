
#include <iterator>

class value_wrapper
{
	public :
		
		bool operator<=>(const value_wrapper& rhs) const = default;
		
	private :
	
		unsigned short value_;
};

int main(int, char**)
{
	value_wrapper lhs, rhs;
	
	auto x = lhs < rhs;
}
