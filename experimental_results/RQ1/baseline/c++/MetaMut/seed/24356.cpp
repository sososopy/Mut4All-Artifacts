namespace NS {

template<typename T>
class Holder
{
private:
	void func();

	template<typename>
	friend class User;
};

template class Holder<long>;

template<typename T>
class User
{
public:
	void method() const
	{
		Holder<T> x;
		x.func();
	}
};

} // namespace

// This one is okay, oddly.
// template class NS::Holder<long>;

void Foo()
{
	NS::User<long> decl;
	decl.method();
}
