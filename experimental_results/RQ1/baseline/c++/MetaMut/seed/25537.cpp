
	int main( int argc, char *argv[])
	{
		auto first = [&]() { return argc; };
		auto second = [&]() { return first(); };
		return second();
	}

