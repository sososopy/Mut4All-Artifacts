
struct a {
    int b;
    void f(int x) {
        (void)[=] { (void)(b + x); };
    }
};

