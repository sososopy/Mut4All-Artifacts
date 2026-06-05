template <typename Tp> auto id() { }

template <typename T> void g(T) { }

int main()
{
    // id<int>;
    g(id<int>);
}

