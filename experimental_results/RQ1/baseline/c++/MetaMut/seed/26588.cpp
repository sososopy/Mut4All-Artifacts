
#include <type_traits>

struct unique_ptr
{
    int operator->(){return true;};
};

template <class Tp>
constexpr auto is_pointer_v = std::is_pointer<Tp>::value;

template <class Tp, int* = nullptr>
auto Wrap1(int) -> std::integral_constant<bool, is_pointer_v<decltype(Tp{}.operator->())>>;

template <class Tp>
auto Wrap1(...) -> std::is_pointer<Tp>;

int main() {
  static_assert(!decltype(Wrap1<unique_ptr>(0))::value); // error
  return 0;
}
