
template <typename T>
struct A
{
    void f(const int);
};

template <typename T>
void A<T>::f(int x)
{
    x = 0;
}

void f()
{
    A<float> a;
    a.f(0);
}


