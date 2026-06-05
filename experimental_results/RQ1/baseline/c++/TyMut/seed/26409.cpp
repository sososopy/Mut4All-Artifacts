
	export module io;

	export struct error
	{
		virtual const char* what() const noexcept;
	};

	// io-impl.cpp
	module io;

	const char* error::what() const noexcept
	{
		return "bla";
	}

	// main.cpp
	import io;

	int main()
	{
		error{};
	}

