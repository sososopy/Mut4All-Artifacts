namespace A { class foo {}; }
namespace B { class bar {}; }

A::foo& operator<<(A::foo& f, const B::bar&) { return f; }

namespace C {
    template <typename T>
    T val();

    class asd {};
    A::foo& operator<<(A::foo& f, const asd& in) { return f; }
    
    template <typename T, typename = void>
    struct has_insertion_operator {
        static constexpr bool value = false;
    };

    template <typename T>
    struct has_insertion_operator<T, decltype(val<A::foo&>() << val<T>(), void())> {
        static constexpr bool value = true;
    };
}

int main() { C::has_insertion_operator<B::bar>::value; }

