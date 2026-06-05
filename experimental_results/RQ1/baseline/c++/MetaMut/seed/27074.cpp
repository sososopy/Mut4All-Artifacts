
typedef void (*foo)();
template<foo f>
struct bar {
     static_assert(f!=nullptr,"f == null!");
};

void baz() {}
inline void bax() {  }

bar<baz> ok;
bar<bax> bad; // error: non-constant condition for static assertion
