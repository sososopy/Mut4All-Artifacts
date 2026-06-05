struct integral_constant {
    int value;
};
template <bool> using __bool_constant = integral_constant;
template <bool> struct enable_if;
template <bool _Cond, typename>
using __enable_if_t = typename enable_if<_Cond>::type;
template <typename> struct is_void : __bool_constant<false> {};
struct _TupleConstraints;
template <bool> using _TCC = _TupleConstraints;
struct tuple {
    template <bool _Dummy>
    using _ImplicitDefaultCtor =
            __enable_if_t<_TCC<_Dummy>::__is_implicitly_default_constructible, bool>;
    template <typename _Dummy = void,
            _ImplicitDefaultCtor<is_void<_Dummy>::value>>
    tuple();
};
template <typename...> struct DBusStruct {
private:
    tuple data_;
    int postProcessKeyEventProperty{[] -> DBusStruct<> {
        return {}
    }};
};