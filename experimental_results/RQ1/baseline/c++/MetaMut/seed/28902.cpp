
#include <coroutine>
#include <exception>

struct simple_task
{
	struct promise_type
	{
	public:
		simple_task get_return_object() { return {}; }

		constexpr std::suspend_never initial_suspend() noexcept { return {}; }

		constexpr std::suspend_never final_suspend() noexcept { return {}; }

		void unhandled_exception() noexcept
		{
			std::terminate();
		}

		constexpr void return_void() const noexcept {}
	};
};

// in debug builds it is a checked pointer
template<class T>
class pointer_wrapper
{
public:

	constexpr pointer_wrapper() noexcept = default;

	constexpr pointer_wrapper(T* p) noexcept : p{ p } {}

	constexpr T* operator->() const noexcept { return p; }

	constexpr T& operator*() const noexcept { return *p; }

	constexpr bool operator==(std::nullptr_t) const noexcept { return p == nullptr; }

	constexpr bool operator==(const pointer_wrapper& other) const noexcept { return p == other.p; }

	constexpr operator bool() const noexcept { return p != nullptr; }

	operator pointer_wrapper<const T>() const noexcept
	{
		return pointer_wrapper<const T>{ p };
	}

private:
	T* p = nullptr;
};

struct some_struct
{
	int i = 0;
	pointer_wrapper<some_struct> other;

	void do_ice()
	{
		auto ice_fn = [this]() -> simple_task { other->i = 1; co_return; };
	}
};
