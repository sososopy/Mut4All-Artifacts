

struct E
{
	E() = default;
	E(const E&) = delete;
	E(E&&) = default;
};

int main()
{
	E e;
	try
	{
		// E e; // Not here.
		throw e;
	}
	catch(...)
	{}
}

