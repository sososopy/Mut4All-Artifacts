template<typename> struct property;

template<typename Derived>
struct B {
    void doit() {
        []<typename R, typename... Args>(R (Derived::*)(Args...)) {
            constexpr bool x = requires {
                requires property<R>::value == 3;
            };
        } (&Derived::f);
    }

};

struct A : B<A> {
    int f();
};

void test() {
    A{}.doit();
}