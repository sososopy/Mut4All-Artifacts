
#include <memory>
#include <type_traits>

template <typename T>
struct Wrapper
{
    T val;

    explicit Wrapper(T && v)
      : val(std::move(v)) {}

    explicit Wrapper(T const & v)
      requires std::is_copy_constructible<T>::value
      : val(v) {}
};

template struct Wrapper<std::unique_ptr<int>>;

int main() {}
