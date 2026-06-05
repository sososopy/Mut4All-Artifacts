
#include <tuple>

template <typename M>
struct is_generative_model {
    private:
        template <typename Z> static auto test(int) -> decltype(

                static_cast<std::tuple<size_t, double>        (Z::*)(size_t,size_t) const>      (&Z::sampleOR),
                static_cast<std::tuple<size_t,size_t, double> (Z::*)(size_t,size_t) const>      (&Z::sampleSOR),

                std::true_type()
                );

        template <typename Z> static auto test(...) -> std::false_type;

    public:
        enum { value = std::is_same<decltype(test<M>(0)),std::true_type>::value };
};

int main() {
    return 0;
}

