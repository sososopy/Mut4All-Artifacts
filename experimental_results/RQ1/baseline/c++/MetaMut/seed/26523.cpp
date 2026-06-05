

class C;

auto x = []<typename MyC = C>(MyC *p) {
    return p->n;
};

class C {
    int n;
    friend decltype(x);
} c;

int k = x(&c);

