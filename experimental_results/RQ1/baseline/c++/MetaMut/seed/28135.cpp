
	struct blub
	{
		char a[5];
	};

	blub f()
	{
		return {"asdf"};  // error: could not convert '{"asdf"}' from '<brace-enclosed initializer list>' to 'blub'
		// return {{'a', 's', 'd', 'f', '\0'}};  // OK
		// return blub {"asdf"};  // OK
	}
