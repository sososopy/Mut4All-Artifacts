

template <class T>
concept bool Destructible() {
    return false;
}

template <class T, class...Args>
concept bool ConstructibleObject =
    Destructible<T>() && requires (Args&&...args) {
        new T{ (Args&&)args... };
    };

int main() {
    using T = int[2][2];
    static_assert(!ConstructibleObject<T, T>);
}

