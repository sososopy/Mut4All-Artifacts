
struct X
{
	friend void f(X, int) = delete;
	friend void f(X, double) {}
};

struct Y;
void g(Y, int);
void g(Y, double);

struct Y
{
	friend void g(Y, int) = delete;
	friend void g(Y, double) {}
};

int main()
{
	X x;
	f(x, 5.0);
	Y y;
	g(y, 5.0);
}

