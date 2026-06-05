
class ClassWithEverythingPrivate {
    class InnerClass {
    public:
        static int InnerFunction() {
            return 123;
        }
    };
};

template <typename T> class TemplateClass {
public:
    static int StaticFunction() {
        return ClassWithEverythingPrivate::InnerClass::InnerFunction();
    }
};

int main() {
    return TemplateClass<void>::StaticFunction();
}
