
using T = int;

struct Wrap
{
    T const& v;
    
    // Shouldn't extend lifetime of temporary
    constexpr Wrap(T const& in) : v{in} {}
};

struct BadWrapUse final
{
    T i{};

    constexpr BadWrapUse()  // issue not caught with constexpr
    // BadWrapUse()  // issue caught without constexpr
    {
        Wrap w{T{}};  // temporary T's lifetime ends after this expression
        i = w.v;      // This should lead to stack-use-after-scope.
    }
};

int main()
{
    BadWrapUse c;
}
