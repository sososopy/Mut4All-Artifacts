template <typename T>
struct Optional {
    Optional(T&&);
};

struct MyClass {
    MyClass(Optional<const void*>);

    static constexpr void* NONE = nullptr;
};

// If NONE moved outside MyClass:
// const void* NONE = nullptr; // Correct Error
// void* NONE = nullptr; // Same Crash
// constexpr void* NONE = nullptr; // Same Crash

void beforeParam();

template<typename T>
struct Create {
    template <typename U> static T create(U &&) noexcept;
};


template <typename T>
template<typename U>
T Create<T>::create(U && u) noexcept {
return T( ( (beforeParam()), (u) ) );

// return T( (u) ); // Correct Error
}

void test_func() {
    Create<MyClass>::create(MyClass::NONE);
}