
    #include <iostream>

    int constructions = 0;
    int destructions = 0;

    struct A
    {
        A()
        {
            constructions++;
        }
        virtual ~A() {
            destructions++;
        }
    };

    struct B : public virtual A
    {
        B(int)
        {
        };

        B() : B(1)
        {
            throw -1;
        }
        virtual ~B() = default;
    };

    struct C : public B
    {
    };

    int main() {
        try
        {
            C c;
        }
        catch (int e)
        {
            std::cout << "Caught: " << e << std::endl;
        }
        std::cout << constructions << " constructions" << std::endl;
        std::cout << destructions << " destructions" << std::endl;
        return 0;
    }

