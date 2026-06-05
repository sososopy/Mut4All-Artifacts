
class Base {};
struct VDerived : virtual Base
{
		VDerived(int x, const char * f, ...) __attribute__((format(printf, 5, 6)));
}  a(1, "%s %d", "foo", 1);
