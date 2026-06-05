
#include <iostream>

struct has_destructor {
	~has_destructor();
};

void do_nothing();

inline int throwing_function() {
#ifdef DESTRUCTOR
	has_destructor hd;
#endif
	throw "";
}

class C {
public:
	C() noexcept;
	~C();
};

C::C() noexcept {throwing_function();}
inline C::~C() { do_nothing(); }

static void term_handler() {
	const char *f = "Died without exception\n";
	auto e = std::current_exception();
	if (e)
		f = "Died with exception\n";
	std::cout << f << std::flush;
	quick_exit(0);
}

int main() {
	std::set_terminate(&term_handler);
	C{};
}
