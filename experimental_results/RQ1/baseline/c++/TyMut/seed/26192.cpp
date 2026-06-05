
class A
{
    class Private{};

public:
    static Private foo()
    {
        return Private();
    }
};

int main()
{
    A::Private normal = A::foo(); // OK: compiler produces error;
    auto error = A::foo(); // ERROR: compiler doesn't see error;
}
