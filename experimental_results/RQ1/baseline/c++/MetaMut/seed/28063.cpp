
#include <boost/optional.hpp>
#include <string>

#if __cplusplus > 201402
#include <type_traits>
#else

namespace std {
// This indirection is needed for some older versions of GCC - including 4.8.5 (CWG 1558)
// template<typename...> using __void_t = void; // would be the natural implementation,
// but the standard did not specify the treatment of unused arguments in an alias template specializations,
// and GCC chose to ignore them, which makes the natural implementation unusable.
template<typename...>
struct voider { using type = void; };

// std::void_t will be part of C++17, but until then define it ourselves:
template<typename... T>
using void_t = typename voider<T...>::type;
// void_t is "just a fancy way to spell void" (Walter E Brown)
// But *ALL* the types have to be *well formed* (that's the magic).

}

#endif

namespace detail {
  // check if a type is boost::optional
  template<typename T, typename = void>
  struct is_boost_optional_impl : std::false_type{};

  template<typename T>
  struct is_boost_optional_impl<
    T,
    std::void_t<decltype(std::declval<T>().is_initialized())>>
   : std::true_type {};
}

namespace rj_hlp {

template<typename T>
struct is_boost_optional : detail::is_boost_optional_impl<T>::type {};

}

struct get_from {
    template<
        typename GetWhat,
        typename = typename std::enable_if<!rj_hlp::is_boost_optional<GetWhat>::value>::type>
    explicit operator GetWhat() const
    {
    	return GetWhat{};
    }

    template<typename GetWhat>
    explicit operator boost::optional<GetWhat> const () const
    {
    	return boost::make_optional(GetWhat{});
    }

	template <typename GetWhat>
	explicit operator boost::optional<GetWhat> () const
	{
		return this->operator boost::optional<GetWhat> const ();
	}


	template <typename AsWhat>
	AsWhat as() const
	{
		return this->operator AsWhat();
	}
};

struct R {
	boost::optional<std::string> password;

	R()
	: password{ get_from().as<decltype(password)>() }
	{}
};

int main()
{
	R r;
}
