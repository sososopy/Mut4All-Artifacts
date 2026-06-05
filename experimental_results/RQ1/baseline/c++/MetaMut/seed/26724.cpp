
struct S1 {};
struct S2 { using t1 = int; };
template<typename T>
concept bool Has_t1() {
    return requires () { typename T::t1; };
}
template<typename T>
struct TS {
    friend void f(TS) {}
    friend void f(TS) requires Has_t1<T>() {} // Unexpected error: redefinition
};
TS<S1> ts1;
TS<S2> ts2;
auto x = []{
             f(ts1);
             f(ts2);
         };

