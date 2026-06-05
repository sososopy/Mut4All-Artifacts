

#include <cstdarg>
struct va_wrap {
	~va_wrap() { if (engaged) va_end(vl); }
	va_list vl;
	operator va_list &() { return vl; }
	bool engaged = false;
};
void f(const char *fmt, ...)
{
	va_wrap va;
	va_start(va, fmt); va.engaged = 1;
	va_arg(va, int);
	if (0)
		va_copy(va, va);
	if (0)
		va_end(va);
}
int main()
{
	f("", 42);
}

