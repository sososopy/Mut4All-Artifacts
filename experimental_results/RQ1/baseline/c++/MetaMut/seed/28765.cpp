
template<typename T>
struct S {
    static int x;
};

namespace {
    template<> int S<int>::x = 0;
}
