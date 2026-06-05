
#include <iostream>
#include <type_traits>

template<typename Type>
class Test
{
    public:
        constexpr Test(const Type val) : _value(val) {}
        constexpr Type get() const {return _value;}
        static void test()
        {
            static constexpr Test<int> x(42);
            std::integral_constant<int, x.get()> i; // This is not working
            std::cout<<i<<std::endl;
        }
    protected:
        Type _value;
};

int main(int argc, char *argv[])
{
    static constexpr Test<int> x(42);
    std::integral_constant<int, x.get()> i; // This is working
    std::cout<<i<<std::endl;
    Test<double>::test();
    return 0;
}
