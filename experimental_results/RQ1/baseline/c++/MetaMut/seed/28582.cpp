
template<int N, typename T, typename... U>
struct tuple_impl : tuple_impl<N + 1, U...>
{ };

template<int N, typename T>
struct tuple_impl<N, T>
{ };

template<typename T, typename U> struct tuple : tuple_impl<0, T, U> { };

template<typename T, int N, typename... U>
void
get(const tuple_impl<N, T, U...>&)
{ }

enum class E1 {a};
enum class E2 {b,c};

template < auto >
struct S
{
    int i;
};

int main()
{
   tuple<S<E1::a>,S<E2::b>> x;
   get<S<E2::b>>(x); // does not compile
}

