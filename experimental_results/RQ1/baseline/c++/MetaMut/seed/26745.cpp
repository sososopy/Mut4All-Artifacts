
template<typename T1, typename T2>
struct BTT {};
template<typename T>
struct BTT<T,T> {
    using type = int;
};

template<typename T1, typename T2>
using Alias = typename BTT<T1, T2>::type;

template<typename T1, typename T2>
concept bool C() {
    return requires() {
               typename Alias<T1, T2>;
           };
}

template<typename T1, typename T2>
requires C<T1, T2>()
int f();

auto i = f<char, int>(); // error: cannot call function

