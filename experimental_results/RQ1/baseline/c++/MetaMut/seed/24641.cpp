
#include <type_traits>

template<typename ...T>
struct type_list {};

template<typename ...T>
struct make_type_list
{
	using type = type_list<T...>;
};

// The bug disappears if you use make_type_list directly.
template<typename ...T>
using make_type_list_t = typename make_type_list<T...>::type;


struct ContainerEndA {};

template<typename ...Ts>
struct ContainerA
{
    using type = make_type_list_t<Ts..., ContainerEndA>;
};


struct ContainerEndB {};

//template<typename Head, typename ...Lists>    // If you use this line instead of the next a Internal compiler error is generated.
template<typename ...Ts>
struct ContainerB
{
    using type = make_type_list_t<Ts..., ContainerEndB>;
};


int main()
{
    // To see the types in ContainerA<>::type uncomment the following line.
    //
    //ContainerA<>::type::doesnt_exist; 
    //
    // GCC error: ‘doesnt_exist’ is not a member of ‘ContainerA<>::type {aka type_list<ContainerEndB>}’.
    // So according to GCC ContainerA<>::type contains ContainerEndB, but it clearly contains only ContainerEndB!

    
    static_assert(
        // It doesn't matter which types you use to instantiate ContainerA and ContainerB
        std::is_same<ContainerA<int>::type, ContainerB<int>::type>::value,
        "This assert doesn't fail but it clearly should!"
    );
}
