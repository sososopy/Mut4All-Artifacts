
class a {
 private: template <typename b> a &operator<<(b);
};
a c();
template <typename> a fn2() {
 int d;
 return c() << d;
}

