
#include <memory>
struct S {};
struct T : S {};

std::unique_ptr<T> F();
const std::unique_ptr<S> s = F();

