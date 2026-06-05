
struct foo {
    typedef void(*ptr)(int&);
    operator ptr() const;
};

int main()
{
    foo f;
    void* p = 0;
    f(p);
}

