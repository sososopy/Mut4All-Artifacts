
#include <functional>
template<typename T> class A {
public:
    void foo(A<T> a, const std::function<void()>& fn = [](){}) { }
    void bar(A<T> a) { foo(a); }
};
int main() {
    A<int> a;
    a.foo(a);
    a.bar(a);
    return 0;
}

