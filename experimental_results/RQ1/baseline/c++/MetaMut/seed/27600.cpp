
template <typename F> void foo(F const&) {}
template <typename F> void bar(F const&) {}

void bad() {
    int x = 5;
    foo([x]() { 
        bar([x]() mutable {
            x = 10;    // << modify local copy in mutable lambda should be ok 
        });
    });
}
