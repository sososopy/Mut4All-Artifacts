
template<typename>
struct trivial { using type = int; };

template<typename... Xs>
using result = typename trivial<Xs...>::type;

template<typename... Xs>
constexpr result<Xs...> foo(Xs...)
{ return 0; }

int main()
{
    foo(0, 0);
}
