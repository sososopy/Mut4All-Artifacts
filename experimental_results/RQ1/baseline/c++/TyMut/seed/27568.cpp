
constexpr int f()
{
	auto p(new int[10000]);
	delete p;
	return 4;
}

int main()
{
	constexpr auto w(f());
}

//

constexpr int f()
{
	auto p(new int);
	delete[] p;
	return 4;
}

int main()
{
	constexpr auto w(f());
}
