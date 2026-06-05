
struct MyStruct {
    ~MyStruct() {}
};
void foo() {
    goto x;
    MyStruct s;
x:
    return;
}

