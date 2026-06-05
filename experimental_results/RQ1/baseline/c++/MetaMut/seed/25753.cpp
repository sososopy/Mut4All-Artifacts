
    using T = int;

    struct X
    {
        T &&f()
        {
            return static_cast<T &&>(value);
        }
        T &&value;
    };

    int main() {}

