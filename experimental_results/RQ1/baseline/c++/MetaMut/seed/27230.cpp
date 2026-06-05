
struct a{};
template <class A> struct b {
    using c=a;
};

class B {
    template <class X>
    friend class b<X>::c;
};

int main() {
    B b;
}

