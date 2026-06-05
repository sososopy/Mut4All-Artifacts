
namespace A
{

}
generator<int> parse()
{
    namespace B = A;
    co_yield 1;
}

