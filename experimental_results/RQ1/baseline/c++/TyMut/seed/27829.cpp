
#include <array>
#include <variant>


template <typename T>
struct type{};

constexpr bool validate(unsigned i)
{
    return true;
}

template <typename T,
          decltype(getMembers(type<T>{}), false) = false>
constexpr bool validate(const T& object)
{
    for (auto member: getMembers(type<T>{}))
    {
        std::visit([&] (auto memPtr) {
            validate(object.*memPtr);
        }, member);
    }
    return true;
}


struct Foo1
{
   unsigned a;
};

struct Foo2
{
   unsigned f1;
   Foo1 f2;
};


template <typename Class, typename T>
using MemPtr = T (Class::*);


constexpr std::array<std::variant<MemPtr<Foo1, unsigned>>, 1> getMembers(type<Foo1>)
{
   return {
      &Foo1::a,
   };
};

constexpr std::array<
    std::variant<MemPtr<Foo2, unsigned>,
                 MemPtr<Foo2, Foo1>>, 2> getMembers(type<Foo2>)
{
   return {
      &Foo2::f1,
      &Foo2::f2
   };
};

static_assert(validate(Foo2{}), "");
