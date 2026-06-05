
template <typename T>
struct foo
{
};

template <int N, typename T, typename ...Us>
bool bar(T &&, Us &&...)
{
    return false;
}

template <int N, typename T, typename ...Us>
bool bar(foo<T> &&, Us &&...)
{
    return true;
}

int main()
{
    bar<0>(foo<int>(), 1, 2);
}
