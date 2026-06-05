
	using abominable = void() &;
	struct s {
		abominable f;
	};
	abominable s::* ptr = &s::f;
