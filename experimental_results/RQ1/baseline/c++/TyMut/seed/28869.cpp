
struct S{
   S();
   ~S();
};

S& foo(){
    static S s;
    return s;
}

