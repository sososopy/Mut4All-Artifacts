
template<class T> struct ref {
        template<class U> ref<U> dy() const { return ref<U>(); }
};
template<class X, class Y> ref<X> dy(ref<Y> y) {
        return y.dy<X>();
}
