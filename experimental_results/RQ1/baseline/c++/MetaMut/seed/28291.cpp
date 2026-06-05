struct B{};
struct C:private B{
};
struct D:C{
    void show(){
        struct B b;
    }
};
