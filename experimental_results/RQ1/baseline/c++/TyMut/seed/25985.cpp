
#include <vector>

template<class T>
class A
{
    std::vector<T> x;
public:
    A() = default;
    A(const A&) = default;
    A(A&&) = default;
    A& operator=(const A&) = default;
    A& operator=(A&&) = default;
};

extern template class A<double>;

template class A<double>;


int main()
{
    A<double> a;
    A<double> b(a);
    A<double> c((A<double>()));
    a = b;
    a = A<double>();
    return 0;
}






