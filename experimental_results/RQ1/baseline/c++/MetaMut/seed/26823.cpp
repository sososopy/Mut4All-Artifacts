
#include <iostream>
#include <system_error>
#include <cerrno>
#include <cstring>

namespace MyLib {

template<typename... CHARS>
struct Message
{
	constexpr Message(CHARS... chars)
		: str {chars...,'\0'}
	{}

	const char str[sizeof...(CHARS)+1];
};

// we can not specialize a function template parially,
// therefore we have to wrap it statically inside a class
// (which can partially be specialized)
template<unsigned COUNTDOWN_A, unsigned COUNTDOWN_B, unsigned COUNTDOWN_C>
struct CharIndexer
{
	template<unsigned SIZE_A, unsigned SIZE_B, unsigned SIZE_C, typename... CHARS>
	static constexpr auto CreateChars(
			const char (&str_a)[SIZE_A],
			const char (&str_b)[SIZE_B],
			const char (&str_c)[SIZE_C],
			CHARS... chars)
	{
		return CharIndexer<COUNTDOWN_A-1, COUNTDOWN_B, COUNTDOWN_C>::CreateChars(str_a, str_b, str_c, chars..., str_a[SIZE_A-COUNTDOWN_A]);
	}
};

template<unsigned COUNTDOWN_B, unsigned COUNTDOWN_C>
struct CharIndexer<1, COUNTDOWN_B, COUNTDOWN_C>
{
	template<unsigned SIZE_A, unsigned SIZE_B, unsigned SIZE_C, typename... CHARS>
	static constexpr auto CreateChars(
			const char (&str_a)[SIZE_A],
			const char (&str_b)[SIZE_B],
			const char (&str_c)[SIZE_C],
			CHARS... chars)
	{
		return CharIndexer<1, COUNTDOWN_B-1, COUNTDOWN_C>::CreateChars(str_a, str_b, str_c, chars..., str_b[SIZE_B-COUNTDOWN_B]);
	}
};

template<unsigned COUNTDOWN_C>
struct CharIndexer<1, 1, COUNTDOWN_C>
{
	template<unsigned SIZE_A, unsigned SIZE_B, unsigned SIZE_C, typename... CHARS>
	static constexpr auto CreateChars(
			const char (&str_a)[SIZE_A],
			const char (&str_b)[SIZE_B],
			const char (&str_c)[SIZE_C],
			CHARS... chars)
	{
		return CharIndexer<1, 1, COUNTDOWN_C-1>::CreateChars(str_a, str_b, str_c, chars..., str_c[SIZE_C-COUNTDOWN_C]);
	}
};

template<>
struct CharIndexer<1, 1, 1>
{
	template<unsigned SIZE_A, unsigned SIZE_B, unsigned SIZE_C, typename... CHARS>
	static constexpr auto CreateChars(
			const char (&str_a)[SIZE_A],
			const char (&str_b)[SIZE_B],
			const char (&str_c)[SIZE_C],
			CHARS... chars)
	{
		return Message<CHARS...>(chars...);
	}
};

template<unsigned A, unsigned B, unsigned C>
constexpr auto concatenate(const char (&a)[A], const char (&b)[B], const char (&c)[C])
{
	return CharIndexer<A,B,C>::CreateChars(a,b,c);
}

} // namespace MyLib

#if 0
#define THROW_SYSTEM_ERROR(ERROR_NO, CAUSE)			\
do {								\
	constexpr static auto what = MyLib::concatenate(	\
		"in '", __PRETTY_FUNCTION__, "' by '" CAUSE "'");	\
	throw std::system_error(ERROR_NO,			\
			std::system_category(), what.str);	\
} while(0)
#endif


#define THROW_SYSTEM_ERROR(ERROR_NO, CAUSE)				\
	throw std::system_error(ERROR_NO, std::system_category(),	\
		MyLib::concatenate("in '", __PRETTY_FUNCTION__, "' by '" CAUSE "'").str)


int main()
{
	try
	{
		THROW_SYSTEM_ERROR(EINVAL, "testfunc()");
	}
	catch(const std::system_error& exception)
	{
		std::cerr << "Exception " << exception.what() << " (" << exception.code() << ")" << std::endl;
	}

	return 0;
}
