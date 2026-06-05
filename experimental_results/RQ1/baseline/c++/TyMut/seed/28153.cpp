
template<typename T>
class A {
public:
    using type = T::type;
    /*A(T::type a) : mA{a} {}*/ // error: expected ')' before 'a'
    A(type a); // OK
    constexpr void a(T::type a) noexcept { // OK
        mA = a;
    }
    [[nodiscard]] constexpr T::type a() const noexcept { // OK
        return mA;
    }
private:
    T::type mA; // OK
};

template<typename T>
A<T>::A(T::type a) : mA{a} {} // OK


struct B {
    using type = int;
};

int main() {
    A<B> a{20};
    a.a(10);
    return a.a();
}
