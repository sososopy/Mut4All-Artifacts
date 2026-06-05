

#include <type_traits>

struct Base {
    Base() noexcept = default;

    template <typename F, typename DecayF = std::decay_t<F>>
    // If this requires-clause is replaces with an enable_if then the code compiles fine
        requires(!std::is_same_v<DecayF, Base>
                 && std::is_constructible_v<DecayF, F>)
    Base(F&&) {}
};

struct Derived : public Base {
    using Base::Base;
    void operator()() const;
};

class ClassA {
    // The class ClassB must be present for the bug to manifest
    class ClassB;

    // This is the only usage of 'Derived'
    Derived const f;
};

// This class and its contructor must be included for the bug to manifest
class ClassA::ClassB {
    ClassB();
};
