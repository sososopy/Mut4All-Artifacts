
template<typename X> concept bool C = true;

// fine
template<C... X>
void bar(X...)
{}

struct foo {
    // error: invalid use of pack expansion expression
    template<C... X>
    void bar(X...)
    {}
};

int main()
{
    bar();
    foo {}.bar();
}
