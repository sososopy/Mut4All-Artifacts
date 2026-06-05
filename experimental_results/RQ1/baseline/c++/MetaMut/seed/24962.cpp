
#include <tuple>
#include <type_traits>

template <typename T>
struct num_wrapper {
    T value;

    explicit num_wrapper(T v) : value{v} {}

    // Works, if i use this one:
    //explicit num_wrapper(const T &v)        : value{v} {}
    //explicit num_wrapper(std::decay_t<T> v) : value{v} {}


    bool shift_right_and_get_lowest_bit() {
        const bool lowest (value & 1);
        value >>= 1;
        return lowest;
    }
};

template <typename T>
num_wrapper(T&&) -> num_wrapper<std::decay_t<T>>;


int main()
{
    num_wrapper a_wrapper (static_cast<const int>(123));
    while (a_wrapper.value != 0) {
        if (a_wrapper.shift_right_and_get_lowest_bit()) {
            // Do something
        }
    }
}

