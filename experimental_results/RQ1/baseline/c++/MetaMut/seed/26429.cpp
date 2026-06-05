
#include <list>
struct A {
    A(int) {}
    A(const A&) = delete;
    A(A&&) {}
};
int main() {
    std::list<A> v = {1,2,3};
}

