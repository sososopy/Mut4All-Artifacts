
template <int>
struct a
{
        int * const t;
        void f();
};

template <int n>
void a<n>::f()
{
        t = 0;
}
