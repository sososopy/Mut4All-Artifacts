

void foo() noexcept {}

int main() {
    auto fn = &foo;
#if !defined(__cpp_noexcept_function_type)
    static_assert(noexcept(fn()) == false, "");
 #else
    static_assert(noexcept(fn()), "");
#endif
}


