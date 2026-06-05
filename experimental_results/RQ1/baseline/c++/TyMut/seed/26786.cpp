

    #include <functional>

    struct TestClass
    {
        static void do_things( std::function<void()> first = []{},
                               std::function<void()> second = []{} );
    };

    int main()
    {
        TestClass::do_things();
    }

