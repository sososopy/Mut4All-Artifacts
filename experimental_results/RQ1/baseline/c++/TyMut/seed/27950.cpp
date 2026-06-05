
template<typename T, typename U>
  struct common_reference { };

template<typename T, typename U>
  using common_reference_t = typename common_reference<T, U>::type;

template<typename T, typename U> concept foo = true;
template<typename T, typename U> concept bar = true;
template<typename T, typename U> concept baz = true;

template<typename T, typename U>
  concept common_reference_with
    =  foo<common_reference_t<T, U>, common_reference_t<U, T>>
    && bar<common_reference_t<T, U>, common_reference_t<U, T>>
    && baz<common_reference_t<T, U>, common_reference_t<U, T>>;

template<typename T>
  using iter_reference_t = decltype(((T*)0)->f());

template<typename I>
  concept forward_iterator
    = common_reference_with<iter_reference_t<I>&&, typename I::value_type&>;

struct test_range
{
  struct iterator
  {
    using value_type = int;

    char f() const;
  };

  iterator begin();
};

template<typename T>
concept F = requires (T& t) { { t.begin() } -> forward_iterator; };

static_assert( !F<test_range> );


