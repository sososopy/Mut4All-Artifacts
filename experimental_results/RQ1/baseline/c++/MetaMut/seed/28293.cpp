
#include <iostream>
#include <iterator>
#include <sstream>
#include <vector>

class tst_ContainerApiSymmetry
{
public:
    template <typename Container>
    void ranged_ctor_non_associative_impl() const;

    void ranged_ctor_std_vector_int() { ranged_ctor_non_associative_impl<std::vector<int>>(); }
};

template <typename Container>
void tst_ContainerApiSymmetry::ranged_ctor_non_associative_impl() const
{
    using V = typename Container::value_type;
    const V values1[] = { V(0), V(1), V(2), V(0) };

    const Container c5 = [&] {
        {
            std::stringstream ss;
            for (auto &v : values1)
                ss << v << ' ';
            ss.seekg(0);
            return Container(std::istream_iterator<V>{ss},
                             std::istream_iterator<V>{});
        }
    }();
}

int main() {
    tst_ContainerApiSymmetry test;
    test.ranged_ctor_std_vector_int();
    return 0;
}
