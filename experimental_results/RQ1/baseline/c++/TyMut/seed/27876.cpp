
struct B
{
	B() = default;
	B(B&&) { /* user-provided */ };
};
struct D : B {};

B f() { return {}; }

D a { f() };

