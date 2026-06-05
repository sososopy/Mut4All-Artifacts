
template <class _Tp>
struct default_delete;

template <class _Tp>
struct default_delete<_Tp[]>
{
    void operator()(_Tp* __ptr) const
    {
        delete[] __ptr;
    }
};

template <typename _Dp>
struct unique_ptr
{
    ~unique_ptr()
    {
        _Dp deleter;
        deleter(0);
    }
};


template <typename V>
unique_ptr<default_delete<V[]>> foo();

struct Vert {};

int main()
{
    foo<Vert>();
}

