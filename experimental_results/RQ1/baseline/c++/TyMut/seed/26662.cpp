

#include <type_traits>

template <class From, class To>
concept bool ExplicitlyConvertible =
  requires (From&& val) {
    requires std::is_same<From,bool>::value;
    requires std::is_same<To,bool>::value;
    requires std::is_same<decltype(val),bool&&>::value;
    static_cast<bool>((bool&&)val); // Line 9
    static_cast<To>((From&&)val);   // Line 10
  };

template <class T>
concept bool Boolean =
  requires (const T& t) {
    { t } -> ExplicitlyConvertible<bool>;
  };

constexpr bool f(Boolean) { return true; } // Line 19
static_assert(f(true));                    // Line 20

