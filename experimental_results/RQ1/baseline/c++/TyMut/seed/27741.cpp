
template <typename... T>
void foo(T&... ts) {
    [...&us=ts]{};
}

