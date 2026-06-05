
struct A
{
    static const int& f()
    {
        static constexpr int x = 0;
        return x;
    }
};

struct B
{
	static void f1();
	static void f2();
};

void B::f1()
{
}


inline void B::f2()
{
}

template <typename T>
struct C
{
	static void f1();
	static void f2();
};

template <>
void C<int>::f1()
{
}

template <>
inline void C<int>::f2()
{
}

int main()
{
	A::f();
	B::f1();
	B::f2();
	C<int>::f1();
	C<int>::f2();
}
