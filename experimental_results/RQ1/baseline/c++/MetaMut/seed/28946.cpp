
// begin source
class foo
{
public:
    int mem_fn(auto a, int b = 0);
};

inline int foo::
mem_fn(auto a, int b/*= 0*/)
{
    return a + b;
}

int main()
{
    foo x;
    x.mem_fn(1);
}
