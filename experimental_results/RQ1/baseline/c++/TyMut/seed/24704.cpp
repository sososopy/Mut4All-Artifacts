
struct T {
    template<class D> operator D*() const;
};

void f(T x) {
    x < x;
}

