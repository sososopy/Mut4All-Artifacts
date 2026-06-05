struct X
{
    X() { }
private:
    X(const X& r);
};


template <typename... Args>
void f(Args&&... args)
{
    X t(args...);
}


int main()
{
    f();
}

