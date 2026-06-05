

using size_t = decltype(sizeof(0));
void* operator new(size_t, void* p) { return p; }

struct allocator
{
  template<typename T, typename... Args>
    void
    construct(T* p, Args... args)
    noexcept(noexcept(::new((void*)p) T(args...)))
    { ::new((void*)p) T(args...); }

  template<typename T>
    void
    destroy(T* p)
    noexcept(noexcept(p->~T()))
    { p->~T(); }
};

class NotNoexcept {
public:
    NotNoexcept() noexcept(false) {}
    ~NotNoexcept() noexcept(false) {}
};

class Automatic {
public:
    Automatic(int bla) noexcept(false) : Bla(bla) {};

private:
    int Bla;
    NotNoexcept Dummy;
};

int main() {
  alignas(Automatic) unsigned char buf[sizeof(Automatic)];
  allocator a;
  a.construct((Automatic*)buf, 42);
  a.destroy((Automatic*)buf);
}

