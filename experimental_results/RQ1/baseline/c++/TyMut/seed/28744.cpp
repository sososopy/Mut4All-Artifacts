
struct empty {};

union U {
	constexpr U():
		a()
	{
	}

	[[no_unique_address]] empty a;
};

constexpr U u;
