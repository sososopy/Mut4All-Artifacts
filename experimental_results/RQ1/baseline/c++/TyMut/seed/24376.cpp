
#include <type_traits>
#include <utility>

template <typename From, typename To>
class is_list_convertible_helper
{
  template <typename To2>
  static void requires_conversion(To2 t);

  template <typename From2, typename To2,
      typename = decltype(requires_conversion<To2>({std::declval<From2>()}))>
  //                                               ^ Braced initializer
  static std::true_type helper(int);

  template <typename From2, typename To2>
  static std::false_type helper(...);

public:
  using type = decltype(helper<From, To>(0));
};

template <typename From, typename To>
class is_list_convertible
  : public is_list_convertible_helper<From, To>::type
{ };

static_assert(!is_list_convertible<double, int>::value,
    "double -> int is narrowing!");

