
template <typename>
void foo_impl()
{
    int data;
    auto L = [&](){ return data; };
    [&](){ L(); }();
}

void foo()
{
    foo_impl<int>();
}

