template <unsigned Current, unsigned Max>
struct bar
{
	using type = typename bar<Current + 1, Max>::type;
};

template <unsigned Max>
struct bar<Max, Max>
{
	using type = unsigned;
};

int main()
{
	using t = typename bar<0, 5000>::type;
	return 0;
}
