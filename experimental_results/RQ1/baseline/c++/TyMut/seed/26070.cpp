#include <iostream>

using namespace std;

template<typename _Type, typename _CharType = char, typename _Traits = char_traits<_CharType>>
struct Foo {
	static void Bar(basic_istream<_CharType, _Traits>&);
};

template<typename _CharType, typename _Traits>
struct Foo<nullptr_t, _CharType, _Traits> {
	static void Bar(basic_istream<_CharType, _Traits> &ris) {}
};

template<typename _CharType, typename _Traits>
struct Foo<bool, _CharType, _Traits> {
	static void Bar(basic_istream<_CharType, _Traits> &ris) {}
};

int main() {
	Foo<bool>::Bar(cin);
	return 0;
}

