struct A { 
    A(int val){} 
};

int main()
{ 
    A a{ [x=123]{ return x; }() }; 
} 

i