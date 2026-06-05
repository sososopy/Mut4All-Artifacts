
    #include <vector>

    struct MyStruct {
        std::vector<char> vec;
        unsigned b;

        MyStruct(unsigned arg1): vec(b), b(arg1){}
    };

    int main() {
        MyStruct m{1};
    }
