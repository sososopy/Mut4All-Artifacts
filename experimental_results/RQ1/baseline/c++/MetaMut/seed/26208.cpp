
namespace Foo 
{
    template <typename T>
    struct Meow
    {
    };
}

template <>
struct Foo::Meow<int>
{
};

