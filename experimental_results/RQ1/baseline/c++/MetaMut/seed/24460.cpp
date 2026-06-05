
#include <unordered_map>

struct Foo : std::unordered_map<int,int> {
    using std::unordered_map<int, int>::unordered_map;
    // ~Foo() = default; // explicitly adding default destructor fixes the problem
};

struct Bar {
    Bar(Foo f = {}) : _f(std::move(f)) {}
    // any of these constructors also work:
    // Bar(Foo f = Foo()) : _f(std::move(f)) {}
    // Bar(Foo f = {}) : _f(f) {}
    
    Foo _f;
};

int main() {
    Bar b;
    
    // The following code also works as expected
    // Foo f1 = {};
    // Foo f2 = std::move(f1);
}

