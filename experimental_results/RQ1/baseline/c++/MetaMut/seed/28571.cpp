
template <class T>
struct intrusive_ptr
{
  T *ptr = nullptr;
  constexpr explicit intrusive_ptr(T* p) : ptr(p) {
    ++ptr->count_;
  }
  constexpr ~intrusive_ptr() {
    if (ptr->dec() == 0)
//    if (--ptr->count_ == 0)
      delete ptr;
  }
  constexpr intrusive_ptr(intrusive_ptr const& a) : ptr(a.ptr) {
    ++ptr->count_;
  }
};

struct Foo {
  int count_ = 0;
  constexpr int dec() {
    return --count_;
  }
};
template class intrusive_ptr<Foo>;

constexpr void bar(intrusive_ptr<Foo> a) 
{
//  if (a.ptr->count_ != 2) throw 1;
}

constexpr bool foo() {
  intrusive_ptr a(new Foo());
  bar(a);
  return true;
}

static_assert(foo());
