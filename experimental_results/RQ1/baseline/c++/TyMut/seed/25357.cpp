

    template <typename X, typename F>
    auto apply(X x, F f)
    {
        return f(x);
    }

    template <typename Y>
    auto add_value(Y y)
    {
        return [y](auto x)
        {
            return x + y;
        };
    }

    int main()
    {
        apply(1, add_value(2));
    }

