

namespace ns
{

template <typename T>
struct foo
{
        template <typename U>
        friend void bar(U) {}
};

}

int main()
{
        ns::foo<int> f_int;
        bar(f_int);

        ns::foo<double> f_double;
        bar(f_double);
}
