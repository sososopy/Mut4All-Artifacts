template<class...>concept Invokable = true;

struct A {};

template<class T, Invokable<bool(const T)> auto = A{}>
struct Set{};

template<class T>
void asdf()
{
	using A = Set<T>;
}

int main()
{
	using A = Set<bool>;
}