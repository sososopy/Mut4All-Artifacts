
#include <utility>

int main()
{
    using std::pair;
    using std::declval;
    using X = decltype(declval<pair<int&&,int&&>>().first);
    auto f = [](auto && p) -> decltype(auto) //((decltype(p)&&)p).first)
    {
        return ((decltype(p)&&)p).first;
    };
    using Y = decltype(f(declval<pair<int&&,int&&>>()));
}

