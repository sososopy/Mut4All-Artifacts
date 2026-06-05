
#include <set>
#include <vector>

using namespace std;

template<class elements_it>
class my_iterator
{
public:

	my_iterator(elements_it begin, elements_it end) :
		begin_(begin), end_(end), i(0) {}
		
	void f()
	{
		for_each(begin_, end_, [=](typename elements_it::value_type) 
		{
			// segmentation fault with [&], [this], [=]
			// "this" not captured with []
			if(v[i]) return;
		});
	}

private:
	elements_it begin_;
	elements_it end_;
	size_t i;
	vector<int> v;
};

int main(int argc, char* argv[])
{
	set<int> elements = { 1, 2, 3 };

	my_iterator<set<int>::iterator> it(begin(elements), end(elements));
}

