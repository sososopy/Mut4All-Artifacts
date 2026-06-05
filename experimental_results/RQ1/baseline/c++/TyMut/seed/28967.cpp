
template <class T>
struct rp {
    T* operator->() const;
    operator T*() const;
    template <class U> explicit operator U*() const;
};
struct b {};
typedef void (b::*fptr)();
void foo(rp<b> n, fptr h) {
    (n->*h)();
}
