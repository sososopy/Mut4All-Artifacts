
#include <cstddef>
#include <type_traits>
#include <utility>
#include <exception>

///////////////////////////////////////////////////////

class VTable
{
  public:
	virtual void func(int x) = 0; // #1 this line affects behavior
	virtual ~VTable() = default;
};

template <typename T>
class Handler final : public VTable
{
  private:
	static constexpr int S = sizeof(void*);
	std::byte _data[S];

  public:
	template <typename... CArgs>
	Handler(CArgs&&... args) {
		static_assert(sizeof(T) <= S);
		::new (&_data[0]) T(std::forward<CArgs>(args)...);
	}

	void func(int) { return; }

	~Handler() { static_cast<T*>(static_cast<void*>(&_data[0]))->~T(); }
};

///////////////////////////////////////////////////////

struct FBase
{
	static constexpr int S = sizeof(void*) + sizeof(VTable);
	std::byte _data[S];

	~FBase() { static_cast<VTable*>(static_cast<void*>(&_data[0]))->~VTable(); } // #2 this line affects behavior
};

struct F : FBase
{
	template <typename T>
	F(T&& t) {
		static_assert(sizeof(Handler<std::decay_t<T>>) <= S);
		::new (&_data[0]) Handler<std::decay_t<T>>(std::forward<T>(t));
	}

	~F() = default;
};

///////////////////////////////////////////////////////

struct Testor
{
	Testor() = default;
	~Testor() = default;

	Testor(const Testor& /*unused*/) { throw std::bad_exception(); }
};

///////////////////////////////////////////////////////

#include <iostream>

int main() {

	try {
		Testor testor;
		F f{testor};
	}
	catch (const std::bad_exception& e) {
		std::cout << " caught! " << e.what() << std::endl;
	}

	return 0;
}
