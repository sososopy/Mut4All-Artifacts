
namespace hh {
    struct nothing {};
    template<typename A = int>
    struct expression_decomposer {
        auto h()  {
            return expression_decomposer<bool>();
        }
    };
}

template <typename T>
void bar() {
        auto m = hh::expression_decomposer( hh::expression_decomposer{}.h());
        m.~expression_decomposer();
}

void foo() { bar<int>(); }
