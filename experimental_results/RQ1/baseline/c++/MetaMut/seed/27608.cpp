
template<class T>
constexpr int fake_tuple_size_v = 3;
template<int...> struct intseq {};
template<int N> using genseq = intseq<__integer_pack(N)...>;

template<int A, class S = genseq<0 ? A : A>>
struct arith_result
{ };

template<typename T>
auto Mul(const T&)
{
    return [](auto) { return arith_result<fake_tuple_size_v<T>> { }; }(0);
}

auto x = Mul(0);
