                                                                                                                       11:38PM
#include <iostream>

// The following two lines are required to build with clang.
//template <typename T> class UnrelatedClass;
//template <typename T> std::ostream& operator<<(std::ostream&, const UnrelatedClass<T>&);

class TopLevelClass
{
    private:
        template <typename T> class InnerClass;
        template <typename T> friend std::ostream& operator<<(std::ostream&, const TopLevelClass::InnerClass<T>&);

        template <typename T> class InnerClass
        {
            friend std::ostream& operator<< <>(std::ostream&, const TopLevelClass::InnerClass<T>&);

            int _x;
        };

    public:
        void f(std::ostream& ioStream);
};

template <typename T> std::ostream& operator<<(std::ostream& ioStream, const TopLevelClass::InnerClass<T>& iInnerClass)
{
    ioStream << iInnerClass._x << std::endl;

    return ioStream;
}

void TopLevelClass::f(std::ostream& ioStream)
{
    InnerClass<int> aInnerClass;

    ioStream << aInnerClass;
}

int main()
{
    TopLevelClass aTopLevelClass;
    aTopLevelClass.f(std::cout);
}



