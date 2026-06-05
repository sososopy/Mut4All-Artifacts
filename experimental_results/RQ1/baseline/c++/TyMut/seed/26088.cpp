
extern "C" void abort();

enum E {A, B};

long long f(int x) { return x; }
long long f(unsigned x) { return x; }

int main()
{
	E e = (E)-1;
	if (sizeof(e) >= sizeof(long long))  // First check that test makes sense.
		return 0;
	if (((long long)e < 0) != (e < 0))  /* { dg-bogus "comparison is always false" } */
		abort();
	if (((long long)e < 0) != (f(e) < 0))  /* { dg-bogus "comparison is always false" } */
		abort();
}
