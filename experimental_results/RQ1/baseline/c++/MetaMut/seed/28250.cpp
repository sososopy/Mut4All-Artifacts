
res notok(bool cond) {
    co_return cond ? res{co_await foo_error()} : res{5};
}
