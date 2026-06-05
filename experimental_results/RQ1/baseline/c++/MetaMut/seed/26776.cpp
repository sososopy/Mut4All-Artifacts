
    template <typename T>
    struct hold {
        T value;
        T&& operator()() && {
            return static_cast<T&&>(value);
        }
    };

    bool result = hold<bool&&>{true}();


