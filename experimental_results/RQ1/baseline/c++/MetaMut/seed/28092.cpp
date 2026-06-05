
class Constant
{
    public:
    constexpr operator T() const { return v; }

    constexpr auto operator()() const {return v;}
};

template<class T, int N>
class Array
{
};

class Test
{
    public:
    template<int C>
    using Cores = Array<int, C % Constant<int, 6>{}>;
};

int main()
{
}
