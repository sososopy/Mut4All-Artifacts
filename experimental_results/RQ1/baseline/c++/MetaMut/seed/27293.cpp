
#include <iostream>
#include <type_traits>
#include <typeinfo>
#include <array>

template<typename T, T... Value> class A {
};

template<typename T, template<T...> class, T...> struct args_union;
template<typename T, template<T...> class TemplateType, T Head, T... Tail> struct args_union<T, TemplateType, Head, Tail...> {
	template<T...> struct args_union_sub;
	template<T Head2, T... Tail2> struct args_union_sub<Head2, Tail2...> {
		template<T... Args> using template_type =								
			std::conditional_t<(Head2 < Head), TemplateType<Head2, Args...>,	// if (Head2 < Head) TemplateType<Head2, Args...>;
			TemplateType<Head, Args...>>;										// else TemplateType<Head, Args...>;

		using type = std::conditional_t<Head == Head2,														// if (Head == Head2)
			typename args_union<T, template_type, Tail...>::template args_union_sub<Tail2...>::type,		//    args_union、args_union_sub
			std::conditional_t<(Head < Head2),																// else if (Head < Head2)
			typename args_union<T, template_type, Tail...>::template args_union_sub<Head2, Tail2...>::type,	//    args_union
																											// else
			typename args_union<T, template_type, Head, Tail...>::template args_union_sub<Tail2...>::type>>;//    args_union_sub
	};
	template<> struct args_union_sub<> {
		template<T... Args> using template_type = TemplateType<>;
		using type = TemplateType<Head, Tail...>;
	};
};

template<typename T, template<T...> class TemplateType> struct args_union<T, TemplateType> {
	template<T...> struct args_union_sub;
	template<T Head2, T... Tail2> struct args_union_sub<Head2, Tail2...> {
		template<T... Args> using template_type = TemplateType<Head2, Tail2..., Args...>;
		using type = TemplateType<Head2, Tail2...>;
	};
	template<> struct args_union_sub<> {
		template<T... Args> using template_type = TemplateType<>;
		using type = TemplateType<>;
	};
};

template<typename T> struct Asub {
	template<T... Value> using type = A<T, Value...>;
};

template<typename T, T... Value1, T... Value2> constexpr auto tmp_union(const A<T, Value1...>&, const A<T, Value2...>&) {
	return typename args_union<T, Asub<T>::template type, Value1...>::template args_union_sub<Value2...>::type();
}

int main() {
	A<int, 0, 1, 4, 6> a;
	A<int, 1, 2, 4, 5> b;  
    std::cout << std::boolalpha << std::is_same_v<decltype(tmp_union(a, b)), A<int, 0, 1, 2, 4, 5, 6>> << std::endl;
	std::cout << typeid(tmp_union(a, b)).name() << std::endl;
}
