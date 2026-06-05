

#include <utility>

using function_type = decltype(std::declval<void (*)(void*) noexcept>());

struct S {
  function_type fptr = nullptr;
};

int main() {
    S thing;
}
