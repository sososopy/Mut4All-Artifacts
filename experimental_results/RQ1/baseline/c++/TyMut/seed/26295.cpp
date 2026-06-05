
float f() { return 0.f; }

template<typename F>
void take_f(F&& f) {}

int main()
{
	// this is OK. reference collapsing 
	take_f(f);

	// next line compiles but it should not: f is not an rvalue !
	take_f<float(void)>(f);

	return 0;
}
