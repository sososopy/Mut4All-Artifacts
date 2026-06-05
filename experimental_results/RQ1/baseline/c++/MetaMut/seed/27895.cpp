
template<typename T>
struct foo {
    T t;

    template<typename U>
    void set(U&& u) requires requires { { static_cast<U&&>(u)} -> T; }
    {
        t = static_cast<U&&>(u);
    }
};

int main()
{
    auto w = foo<int>{5};
    w.set(5.0f);
}
