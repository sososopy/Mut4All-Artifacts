
#include <variant>

struct foo : std::variant<long, int>
{
    using std::variant<long, int>::variant;
    using std::variant<long, int>::operator=;
};

//using workaround = decltype(foo{*static_cast<foo const*>(0)});

struct bar
{
    bar& operator=(foo ve)
    {
        value = std::move(ve);
        return *this;
    }

    foo value;
};

    
int main()
{
    foo a;
    foo b{a};
}

