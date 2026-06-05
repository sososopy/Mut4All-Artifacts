
#include <functional>
#include <type_traits>
#include <utility>

template <class T>
inline constexpr bool is_reference_wrapper_v = false;

template <class T>
inline constexpr bool is_reference_wrapper_v<std::reference_wrapper<T>> = true;

template <class T, class Fn>
constexpr
auto my_mem_fn(Fn T::*pmf) {
    return [pmf]<class U, class ...ARGS>(U&& p, ARGS&& ...args) -> decltype(auto)
    {
        if constexpr(is_reference_wrapper_v<std::remove_reference_t<U>>) {
            return operator()(pmf, p.get(), std::forward<ARGS>(args)...);
        }
        else {
            return 12;
        }
    };
}

struct Dummy {
    constexpr int twelve() const { return 12; }
};

int main() {
   Dummy x{};
   auto y = my_mem_fn(&Dummy::twelve);
   int c = y(std::ref(x));
}
