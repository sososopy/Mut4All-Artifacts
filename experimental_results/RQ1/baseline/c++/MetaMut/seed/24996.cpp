

    template<typename Derived>
    struct Base {
        static constexpr int i = Derived::j;
        // static_assert(i>1, "First assert"); // First Assert
        // int array[i]; // Array declation
    };

    struct Derived : public Base<Derived> {
        static  constexpr int j = 5 ;
    };

    static constexpr int k = Base<Derived>::i; 
    static_assert(Derived::i > 0, "Second Assert");
    static_assert(Base<Derived>::i > 0, "Third Assert");
    int array[Derived::i];

