
volatile int x, y;

void f(bool b) {
	(b ? x : y) = 27;
}

