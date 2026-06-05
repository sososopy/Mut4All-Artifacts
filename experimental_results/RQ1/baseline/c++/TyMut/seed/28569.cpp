
Task Foo(auto && ... args) noexcept;

int v;
Foo(v, 2134);

void* operator new(std::size_t len, auto && ...args) noexcept;
