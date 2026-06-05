
template <unsigned long>
struct e
{
};
template <long f>
e<f> h;
template <typename...>
class ad;
long bi;
struct i
{
	static constexpr bool bm = 0;
};
template <typename...>
union ap {
};
template <typename ag, typename... ah>
union ap<ag, ah...> {
	constexpr ap(e<0>) : aq() {}
	template <unsigned long af>
	constexpr ap(e<af>) : bt(h<af - 1>)
	{
	}
	ag aq;
	ap<ah...> bt;
};
template <bool, typename...>
struct as;
template <typename... bv>
struct as<false, bv...>
{
	template <unsigned long af>
	constexpr as(e<af>) : av(h<af>), aw(af)
	{
	}
	void j() { aw = bi; }
	~as() { j(); }
	ap<bv...> av;
	int aw;
};
template <typename... bv>
using az = as<i::bm, bv...>;
template <int, typename... bv>
struct k : az<bv...>
{
	using bb = az<bv...>;
	bb::bb;
};
template <typename... bv>
using ce = k<0, bv...>;
template <int, typename... bv>
struct m : ce<bv...>
{
	using bb = ce<bv...>;
	bb::bb;
};
template <typename... bv>
using be = m<0, bv...>;
template <int, typename... bv>
struct p : be<bv...>
{
	using bb = be<bv...>;
	bb::bb;
};
template <typename... bv>
using bg = p<0, bv...>;
template <int, typename... bv>
struct q : bg<bv...>
{
	using bb = bg<bv...>;
	bb::bb;
};
template <typename... bv>
using ck = q<0, bv...>;
template <typename... bv>
struct r : ck<bv...>
{
	using bb = ck<bv...>;
	template <unsigned long af>
	constexpr r(e<af> s) : bb(s)
	{
	}
};
template <typename, typename>
struct l;
template <typename g, typename... bv>
struct l<g, ad<bv...>>
{
	static constexpr long c = 1;
};
template <typename... bv>
class ad : r<bv...>
{
	using bb = r<bv...>;
	template <typename g>
	static constexpr long l = l<g, ad>::c;

public:
	template <typename g>
	constexpr ad(g) : ad(h<l<g>>)
	{
	}
	template <unsigned long af>
	constexpr ad(e<af>) : bb(h<af>)
	{
	}
};
template <int>
struct n
{
	double d = 1.;
};
using ch = ad<int, n<1>>;
main()
{
	ch const o{n<0>()};
}
