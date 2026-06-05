
namespace std
{
  struct true_type { static constexpr bool value = true; };
  struct false_type { static constexpr bool value = false; };

  template<bool Cond, typename T = void> struct enable_if { using type = T; };
  template<> struct enable_if<false> { };

  template<typename T, typename U> struct is_same : false_type { };
  template<typename T> struct is_same<T, T> : true_type { };

  template<bool C, typename If, typename Else>
    struct conditional
    {
      using type = If;
    };

  template<typename If, typename Else>
    struct conditional<false, If, Else>
    {
      using type = Else;
    };

  template<typename If, typename Else>
    struct __and_
    : conditional<If::value, Else, If>::type
    { };

  template<typename If, typename Else>
    struct __or_
    : conditional<If::value, If, Else>::type
    { };

  template<typename T>
    struct __not_
    : conditional<T::value, false_type, true_type>::type
    { };

  template<typename T>
    T declval();
}

  template<typename _Sig> class function;

  template<typename _Res, typename _Arg>
    class function<_Res(_Arg)>
    {
      typedef _Res _Signature_type(_Arg);

      template<typename _Functor>
	using _Invoke
	= decltype(std::declval<_Functor&>()(std::declval<_Arg>()));

      // Used so the return type convertibility checks aren't done when
      // performing overload resolution for copy construction/assignment.
      template<typename _Tp>
	using _NotSelf = std::__not_<std::is_same<_Tp, function>>;

      template<typename _Functor>
	using _Callable
	  = std::__and_<_NotSelf<_Functor>,
		   std::is_same<_Invoke<_Functor>, _Res>>;

      template<typename _Cond, typename _Tp>
	using _Requires = typename std::enable_if<_Cond::value, _Tp>::type;

    public:
      function() noexcept { }

      function(const function&) { }

      template<typename _Functor,
	       typename = _Requires<_Callable<_Functor>, void>>
	function(_Functor) { }

      _Res operator()(_Arg) const;
    };

struct Foo {
  using Func = function<void(Foo)>;

  static const Func DefaultFunc;

  Func CurrentFunc = DefaultFunc;
};
