

class Base {
    public:
        virtual void foo()=0;
};

template <typename T>
class MiddleA : virtual public Base {
    public:
        virtual void foo() override {}
};

class MiddleB : virtual public Base {};

template <typename T>
class Derived final : public MiddleA<T>, public MiddleB {
    public:
        void bar()
        {
            this->foo();
        }
};

int main()
{
    auto a = Derived<void>();
    a.bar(); // Instantiate the template
}

