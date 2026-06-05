
#include <type_traits>

template<typename T>
struct hana {
  template<typename U>
  consteval bool operator==(hana<U>) {
    return std::is_same_v<T, U>;
  }
};

int main ()
{
  static_assert(hana<int>{} == hana<int>{});
}
