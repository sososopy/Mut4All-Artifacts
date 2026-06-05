
class MyClass
{
 public:
    static constexpr int func(int);
};

constexpr int MyClass::func(int i) { return i+1; }
