
template<bool b>
struct s {
    void f() const;
    static void f() requires b;
};

void g() {
    s<true>().f();
}

