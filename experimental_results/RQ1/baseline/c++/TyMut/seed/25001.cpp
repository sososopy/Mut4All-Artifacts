
template <bool, typename> struct enable_if;
template <CharT, CharT... String>
typename enable_if<sizeof...(String) == 3, int>::type operator"" _script();
