
struct bind_in
{
    template <typename R>
    operator R const& () const;
}a;
int x(void) {
    return a;
}

