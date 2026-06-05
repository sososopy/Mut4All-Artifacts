
#include <memory>

struct A
{
	void F() const
	{
		F_impl();
	}

	virtual std::unique_ptr<int> F_impl() const = 0;
};

template <typename Derived>
	struct A_impl : virtual A
{
	std::unique_ptr<int> F_impl() const override
	{
		dynamic_cast<const Derived &>(*this);
		return nullptr;
	}
};

struct B : virtual A {};

struct C : B, A_impl<C> {};

int main()
{
	C().F();
}

