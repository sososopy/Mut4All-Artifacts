

    template <typename T> struct Foo;
    template <typename T> int variable_template         = 0;
    template <typename T> int variable_template<Foo<T>> = 0;
    template <typename T> int get_variable_template() { return variable_template<T>; }

    int main() {
        get_variable_template<Foo<int>>();
    }


