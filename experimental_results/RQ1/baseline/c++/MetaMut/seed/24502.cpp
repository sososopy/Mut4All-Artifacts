
template <class T>
class mask_destructor {
private:
    union {
        alignas(T) char bytes[sizeof(T)];
        T obj;
    };
public:
    T& get() const { return obj; }
    // preserve T's constexpr constructor
    constexpr mask_destructor() : obj() { }
    mask_destructor(const mask_destructor &other) : obj(other.obj) { }
    mask_destructor &operator=(const mask_destructor &other){
        obj = other.obj;
        return *this;
    }
    // Note no destructor defined. The compile implicitly deletes it
    // because of the anonymous union above.
};
