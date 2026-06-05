
#include <memory>
#include <coroutine>

template <std::movable T>
class Generator
{
public:
    struct promise_type
    {
        Generator<T> get_return_object();
        void return_void() noexcept { }
        void unhandled_exception() noexcept;
        static std::suspend_always initial_suspend() noexcept { return {}; }
        static std::suspend_always   final_suspend() noexcept { return {}; }
        std::suspend_always yield_value(T v) noexcept;
    };
};

struct MyStruct
{
    MyStruct(std::initializer_list<std::shared_ptr<int>> dst);
};


Generator<MyStruct> add(std::shared_ptr<int> dest)
{
    //MyStruct tmp({dest});
    //co_yield tmp;
    co_yield MyStruct({dest});
}
