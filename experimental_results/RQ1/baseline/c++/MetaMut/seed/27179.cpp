

template<typename T>
struct Base {
    auto f(int) { return static_cast<T*>(this); }
    auto f(char) { return static_cast<T*>(this); }
};

struct A : Base<A> {};

int main() {
    A instance;

    // instance.f('x') // "fixes" the issue

    auto ptr = static_cast<A* (A::*)(char)>(&A::f); // error here
    (instance.*ptr)('x');
}


