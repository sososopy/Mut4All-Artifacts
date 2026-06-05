
namespace std
{
typedef int size_t;
template <typename _Tp> struct numeric_limits
{
  static constexpr _Tp
  max ()
  {
    return 0;
  }
};
}

template <typename> using identity_transform = int;
template <template <typename> class> struct transform_sequence;
namespace detail
{
namespace recursive_transform_impl
{
template <int, int, template <typename> class, template <typename> class,
          template <typename> class,
          template <typename, template <typename> class> class,
          template <typename> class>
struct recurse;
}
}
template <template <typename> class TPredicate,
          template <typename, template <typename> class> class TTransformer,
          template <typename> class... TTransforms>
struct A
{
  template <template <typename> class TPreTransform = identity_transform,
            template <typename> class TPostTransform = identity_transform,
            std::size_t = std::numeric_limits<std::size_t>::max ()>
  using with = detail::recursive_transform_impl::
      recurse<0, 0, TPreTransform, TPostTransform, TPredicate, TTransformer,
              transform_sequence<TTransforms...>::template apply>;
  using apply = typename with<>::template apply<>;
};

