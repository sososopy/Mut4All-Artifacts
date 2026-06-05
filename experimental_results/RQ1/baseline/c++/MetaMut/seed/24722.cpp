
template <typename R, typename ...Args>
using zip_t = zip<R, Args...>::with<Args...>;


template<typename...> struct Tuple {};
template<typename T1, typename T2> struct Pair {};

template<class R,class ...Args1>  struct zip {
    template<class ...Args2> struct with {
        R flag;
        typedef Tuple<Pair<Args1, Args2>...> type;
    };
};

typedef zip<bool,short, int>::with<unsigned short, unsigned>::type T1;
T1 make_zip1(bool) {return T1();}

template <typename R, typename ...Args>
using zip_t = zip<R, Args...>::with<Args...>;

template<typename A>
static zip_t<A>::type make_zip(A) {
  return zip_t<A>::type{};
}


//test code
int main() { return 0; }
