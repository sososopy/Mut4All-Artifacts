
template<typename T>
  T&& declval() noexcept;

template<bool B>
struct bool_constant
{
  static constexpr bool value = B;
  using type = bool_constant;
};

using true_type = bool_constant<true>;
using false_type = bool_constant<false>;

template<bool, typename T, typename Arg>
  struct __is_nt_constructible_impl
  : public false_type
  { };

template<typename T, typename Arg>
  struct __is_nt_constructible_impl<true, T, Arg>
  : public bool_constant<noexcept(static_cast<T>(declval<Arg>()))>
  { };

template<typename T, typename Arg>
  using __is_nothrow_constructible_impl
    = __is_nt_constructible_impl<__is_constructible(T, Arg), T, Arg>;

template<typename T>
  struct __is_nothrow_copy_constructible_impl
  : public __is_nothrow_constructible_impl<T, const T&>
#ifdef FIX
    ::type
#endif
  { };

template<typename T>
  struct is_nothrow_copy_constructible
  : public __is_nothrow_copy_constructible_impl<T>
#ifndef FIX
    ::type
#endif
  { };

struct NType
{
  NType();
private:
  NType(const NType&);
};

static_assert( !is_nothrow_copy_constructible<NType>::value, "" );
