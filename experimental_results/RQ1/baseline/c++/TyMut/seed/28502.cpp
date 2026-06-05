
struct s{
    int* i;
    s();
    ~s(){ delete i;}
};


void bar(){
    s instance;
    s instance2 = instance;
}
