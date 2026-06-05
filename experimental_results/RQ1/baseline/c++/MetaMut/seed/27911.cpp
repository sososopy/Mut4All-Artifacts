#include <type_traits>

struct A
{
  static constexpr std::true_type value {};
};

int main()
{
  A a;
  A& ref = a;
  constexpr bool r1 = a.value; //ok
  constexpr bool r2 = ref.value; // this should not work
}

