
//bug_a.ii
template<typename... _Tp> struct common_type;

template<> struct common_type<> {};

template<typename _Tp0> struct common_type<_Tp0> {};

template<typename _Tp1, typename _Tp2> struct common_type<_Tp1, _Tp2> {};

//bug_b.ii
import  "bug_a.ii";

template<typename _Rep1, typename _Rep2,
	 typename _CRep = typename common_type<_Rep1, _Rep2>::type>
struct X;

