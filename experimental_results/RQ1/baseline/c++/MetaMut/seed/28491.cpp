
template<class F>
auto toStatic(int i, F f) {
    switch(i) {
        case 0: return f( std::integral_constant<int, 0>{} );
        case 1: return f( std::integral_constant<int, 1>{} );
        case 2: return f( std::integral_constant<int, 2>{} );
        default: assert("too big");
    }
}

struct Base {
    virtual void show() const = 0;
};
template <size_t I> struct F {
    template <size_t J> struct K : Base {
        void show() const override {
            std::cout << I << " " << J << std::endl;
        }
    };
};


void show(int i, int j) {
    return toStatic(i, [j](auto I) {
        using A = F<I>;
        // using A = F<decltype(I)::value>; // this works
        return toStatic(j, [](auto J)  {
            using impl = typename A::template K<J>;
            impl{}.show();
        });
    });
}


