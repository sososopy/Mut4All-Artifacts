struct A{
    A(){}
};
union C{
   A a;
   int b = 0;
};
int main(){
    C c;
} 

