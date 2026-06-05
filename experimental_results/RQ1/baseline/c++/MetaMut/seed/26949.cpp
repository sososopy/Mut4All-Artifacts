
namespace std {
template <typename> struct remove_cv;
template <typename> struct is_reference;
template <typename> void declval();
template <typename> struct is_constructible;
template <typename> struct is_nothrow_constructible;
template <typename _Tp> using remove_cv_t = typename remove_cv<_Tp>::type;
namespace meta {
template <typename> struct Trans_NS_extension_apply_list;
template <typename T> using _t = typename T::type;
namespace ext {
template <class> void ImplicitlyConvertibleTo();
}
template <class> void Assignable();
namespace ext {
template <class T, class... Args> int ConstructibleObject = requires { T{}; };
template <class T, class... Args>
concept int BindableReference =
    std::is_reference<T>::value &&std::is_constructible<T>::value;
template <class T, class... Args> concept bool Constructible() {
  return ext::ConstructibleObject<T> || ext::BindableReference<T, Args...>;
}
template <class T> concept bool DefaultConstructible() {
  return Constructible<T>() && requires { new T[0]; };
}
template <class T> concept bool MoveConstructible() {
  return Constructible<T>() && ext::ImplicitlyConvertibleTo<T>;
}
template <class T> concept bool Movable() {
  return MoveConstructible<T>() && Assignable<T &&>;
}
namespace detail {
template <class, class> int Swappable_ = requires { 0; };
template <class T, class U> int Swappable();
template <class T> concept int Dereferencable = requires{{0}};
}
template <detail::Dereferencable R> using RvalueReferenceType = decltype(0);
namespace detail {
template <class T> int IsValueType;
template <class> struct value_type;
template <class T>
requires detail::IsValueType<
    meta::_t<value_type<std::remove_cv_t<T>>>> using ValueType =
    meta::_t<value_type<std::remove_cv_t<T>>>;
template <class I> concept bool Readable() {
  return Movable<I>() && DefaultConstructible<I>() &&
         detail::Dereferencable<const I> && requires{{0}};
}
template <class Out, class T> concept bool MoveWritable() {
  return Movable<Out>() && DefaultConstructible<Out>() &&
         detail::Dereferencable<Out>;
}
template <class In, class Out> concept bool IndirectlyMovable() {
  return Readable<In>() && Movable<ValueType<In>>() &&
         Constructible<ValueType<In>>() &&
         MoveWritable<Out, RvalueReferenceType<In>>() &&
         MoveWritable<Out, ValueType<In>>();
}
IndirectlyMovable { In, Out }
int is_nothrow_indirectly_movable_v =
    is_nothrow_constructible<ValueType<In>>::value;
template <Readable R1, Readable R2>
    requires IndirectlyMovable<R1, R2>() &&
    IndirectlyMovable<R2, R1>() void iter_swap2();
}
}
}
}

