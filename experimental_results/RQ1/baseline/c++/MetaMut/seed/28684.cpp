template<bool> struct enable_if { };
template<> struct enable_if<true> { using type = void; };

template<bool B>
using enable_if_t = typename enable_if<B>::type;

struct tuple { };
struct pair { };

template<unsigned N> enable_if_t<N == 1> get(tuple&) { }
template<unsigned N> enable_if_t<N == 1> get(const tuple&) { }
template<unsigned N> enable_if_t<N == 1> get(pair&) { }
template<unsigned N> enable_if_t<N == 1> get(const pair&) { }

template<int N>
constexpr unsigned
frob()
{
  static_assert(N == 1, "user-friendly diagnostic");
  return unsigned{N}; // narrowing check, reject negative values
}

template<int N> void get_n(tuple& t) { get<frob<N>()>(t); }

int main()
{
  tuple t;
  get_n<-1>(t);
}
