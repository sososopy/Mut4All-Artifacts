
decltype(auto) f(auto x) {
    return x[0];
}

const int x[] = {0};

extern decltype(f(x)) a;
extern const int& a;

