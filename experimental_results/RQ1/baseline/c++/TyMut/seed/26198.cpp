
int main()
{
	int t = 0;
	return [&]() -> int {int __t; __t = t; return __t; }();
}

