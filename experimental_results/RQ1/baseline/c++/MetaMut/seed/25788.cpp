
struct X
{
	//friend void f(int) {} // #1
	template <class T> friend void f(T) {} // #2
};

int main()
{
	f(5); // #3
}
