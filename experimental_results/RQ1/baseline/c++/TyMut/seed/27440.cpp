
struct C {
    void method() {}
};

void f()
{
#pragma GCC diagnostic push                                                                                                                                                          
#pragma GCC diagnostic ignored "-Wnonnull" 
    static_cast<C*>(0)->method();
#pragma GCC diagnostic pop
}
