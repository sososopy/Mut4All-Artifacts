
#include <functional>

void elem_func(int) {}

template <
    typename Sequence,
    std::invocable<int> SequenceHandler
>
void func(
    const Sequence& values,
    SequenceHandler&& handler,
    std::enable_if_t<std::is_pointer_v<Sequence>, int> = 0)
{
}

template <
    typename T,
    std::invocable<T> ValueHandler
>
void func(
    T value,
    ValueHandler&& handler,
    std::enable_if_t<!std::is_pointer_v<T>, int> = 0)
{
}

int main()
{
    func(
        new int[1], // pointer => 1st func should be called 
        [](auto e)  // lambda should be std::invocable<int> => decltype(e)==int
        {
            elem_func(e); // cannot call elem_func if decltype(e)==int* (2nd func)
        }
    );
}
