
typedef unsigned long size_t;

template<class T, size_t N = -1>
struct X
{
    T t[N];
};

template<class T>
struct X<T, -1>
{
    T* t;
};

X<int> x;

