
#include <iostream>
#include <utility>

struct PairPtr {

    PairPtr() {}

    PairPtr(const PairPtr &s) {
        a = s.a;
        b = s.b;
    }

    explicit PairPtr(int *_a, int *_b) {
        a = _a;
        b = _b;
    }

    PairPtr& operator=(const PairPtr &s) {
        a = s.a;
        b = s.b;
        return *this;
    }

    PairPtr& operator=(const std::pair<int *, int *>& pair) {
        a = pair.first;
        b = pair.second;
        return *this;
    }

    int *a;
    int *b;
};

void func(int *a, int *b)
{
    PairPtr p;

    p = { a, b };
}

