
struct AutoPtr {
    AutoPtr() = default;
    AutoPtr(AutoPtr&) {}
};
template<class T> auto f(T p, int) -> decltype(throw p, 1) { throw p; }
template<class T> int f(T p, long) { return 2; }
int main() {
    return f(AutoPtr(), 42);
}

