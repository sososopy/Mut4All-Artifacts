
template <typename T>
struct ptr {
    T *p;
    ptr() : p() {}
    ptr(ptr &) = delete;
    ptr(ptr &&o) : p(o) {}
    operator T *() { return p; }
};

static ptr<int> f()
{
    ptr<int> pt;
    #pragma omp task shared(pt)
    pt.p = 0;
    return pt;
}

int main(int c, char **v)
{
    #pragma omp parallel
    #pragma omp single    
    f();
}
