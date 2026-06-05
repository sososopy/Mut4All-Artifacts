
class ClassA
{
    public:
        ClassA() {}
        ClassA(const ClassA& v) {}
        ClassA(ClassA&& v) = delete;
};


ClassA fA() { return ClassA(); } // Here the movement constructor should be selected

int main()
{
    ClassA a { fA() };
    return 0;
}
