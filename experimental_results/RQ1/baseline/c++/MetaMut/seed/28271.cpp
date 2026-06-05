

template<bool B>
struct bool_constant { static constexpr bool value = B; };

template<typename From, typename To>
struct is_convertible
: public bool_constant<__is_convertible(From, To)>
{ };

class Private
{
  operator int() const;
};

static_assert( not is_convertible<Private, int>::value, "" );

