
int main(int, char**)
{
	thread_local int foo;
	[&] (auto bar) {
		foo += bar;
	}(1u);
	return 0;
}

