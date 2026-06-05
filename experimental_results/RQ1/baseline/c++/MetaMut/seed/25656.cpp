
    struct base {
        explicit base(int) {}
        virtual void foo() = 0;
        virtual ~base() {}
    };

    struct left : virtual base {};
    struct right : virtual base {};

    struct derived : left, right {
        virtual void foo() override {}
        derived() : base(10) {}
    };

    int main() {}
