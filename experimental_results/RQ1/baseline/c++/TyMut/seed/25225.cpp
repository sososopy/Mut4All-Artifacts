

struct Base {
    mutable int i;
};

struct Derived : Base {};

const Derived foo{};

int main() { foo.i = 42; }

