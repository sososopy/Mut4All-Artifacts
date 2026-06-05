

    void test(auto param)
    requires requires{ { [](auto p){return p;}(param) }; };

    void test2() { test(1); }
