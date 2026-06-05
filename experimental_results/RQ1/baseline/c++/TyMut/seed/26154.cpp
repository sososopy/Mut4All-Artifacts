

#include <string>
#include <functional>
using namespace std;
using namespace std::placeholders;

int main()
{
    pair<string, int> table = {"0123456789012345", 0};
    auto get_first = mem_fn(&pair<string, int>::first);

    auto it = bind(greater<size_t>(), bind(&string::length, bind(get_first, _1)), 5);
    it(table);
}
