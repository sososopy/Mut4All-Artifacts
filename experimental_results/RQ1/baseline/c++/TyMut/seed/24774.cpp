
struct A {
    void f(this A);

    // bug: no error
    // correct behavior: must be error
    template <typename... Args>
    void g(this Args&&...);

    template <typename... Args>
    void h(this A, Args&&...);
};

