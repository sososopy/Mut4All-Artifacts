
    // g++ -std=c++2a -o /tmp/example.cpp.o -c example.cpp
    enum class MyEnumClass { A };
    struct MyClass {
        MyEnumClass foobar : 8 { MyEnumClass::A };
    };
    bool some_function(MyClass x)
    {
        switch (x.foobar) {
        case MyEnumClass::A:
            return false;
        }
    }

