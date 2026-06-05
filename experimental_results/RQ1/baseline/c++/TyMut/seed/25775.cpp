
template <class T=void> struct X
{
	friend void f(){
		struct Y{}; // line 6
	}
};

