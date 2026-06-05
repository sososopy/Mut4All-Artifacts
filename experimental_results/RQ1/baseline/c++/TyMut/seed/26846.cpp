

template <int... _is>
struct Expand
{
    static void (*const m_arr[sizeof...(_is)])();
};

template <int... _is>
void (*const Expand<_is...>::m_arr[])() = {
        [] () { (void)_is; } ...
};

int main()
{
    Expand<12,34,56> e;
    e.m_arr[0]();
    e.m_arr[1]();
    e.m_arr[2]();
}
