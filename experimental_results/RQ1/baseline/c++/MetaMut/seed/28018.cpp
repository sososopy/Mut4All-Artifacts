
template <class... T>
void a(const T&...) {}

template <class... T>
void b(const T&... t) {
  [&]() {
    a(t...);
    a(t...);
  };
}

void c() {
  b(1);
}
