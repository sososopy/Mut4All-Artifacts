
struct S { int member; struct member{}; };

template<class T, class = typename T::member>
auto f(int) -> char(&)[1];

template<class T>
auto f(...) -> char(&)[2];

static_assert(sizeof(f<S>(0)) == 2, "");

int main() {
}
