
#include <cstddef>
#include <new>

struct S {
    void* operator new(this std::size_t);
    void operator delete(this S*, std::destroying_delete_t);

    operator S*() const;
    operator std::size_t() const;
};

int main()
{
    S{}.operator new();
    S{}.operator delete(std::destroying_delete);
}

