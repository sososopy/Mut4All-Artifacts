
struct Type
{
  int i;

  Type() : i(0) { }

  int& get() & { return i; }
  int const& get() const& { return i; }
  int volatile& get() volatile& { return i; }
  int const volatile& get() const volatile& { return i; }

  int&& get() && { return std::move(i); }
  int const&& get() const&& { return std::move(i); }
  int volatile&& get() volatile&& { return std::move(i); }
  int const volatile&& get() const volatile&& { return std::move(i); }
};

