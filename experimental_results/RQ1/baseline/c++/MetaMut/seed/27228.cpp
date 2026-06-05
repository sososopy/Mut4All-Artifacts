
#ifdef __clang__
#include <experimental/coroutine>
using namespace std::experimental;
#else
#include <coroutine>
using namespace std;
#endif

struct task
	{
	struct promise_type
		{
		auto get_return_object () -> task 
			{
			return {};
			}

		auto initial_suspend () -> suspend_never
			{
			return {};
			}

		auto final_suspend () -> suspend_always
			{
			return {};
			}

		void unhandled_exception ()
			{ }

		void return_void ()
			{}
		};
	};

struct ret_type 
	{
	ret_type () = default;
	ret_type (const ret_type&) =delete;
	};

struct awaiter
	{

	auto await_ready() const -> bool 
		{
		return true;
		}

	void await_suspend (coroutine_handle<>)
		{}

	auto await_resume() -> ret_type
		{
		return {};
		}

	};

task f()
	{
	ret_type r2 {co_await awaiter{}};
	};

int main()
{
auto x = f();
return 0;
}	

