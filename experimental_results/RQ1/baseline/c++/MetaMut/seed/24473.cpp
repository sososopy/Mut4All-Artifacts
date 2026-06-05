
#include <tuple>
#include <iostream>

// designates a field to be serialized
template <typename Name, typename Type>
class MemberPair {
public:
    typedef Type type;
    typedef Name name;
};

template <typename... All>
class A {};

template <typename Base, typename Head, typename... Tail>
class A<Base, Head, Tail...> : A<Base, Tail...> {
public:
    template <typename NameAttemptHead, typename... NameAttemptTail>
    using Type =
        typename
            std::tuple<typename
                std::enable_if<std::is_same<NameAttemptHead,
                                            typename Head::name>::value,
                               typename Head::type>::type,
                typename A<Base, Tail...>::template Type<NameAttemptTail...> >;

};

template <typename Base>
class A<Base> {
public:
    template <typename... Empty>
    using Type = std::tuple<>;
};

namespace F {
    struct x;
    struct y;
    struct z;
}

class D : public A<D, MemberPair<F::x, int>,
                      MemberPair<F::y, int>,
                      MemberPair<F::z, std::string> > {

};

int
main()
{
    // D dee;
    // static_cast< A<D, MemberPair<F::y>, MemberPair<F::z> &>(dee);
    return 0;
}
