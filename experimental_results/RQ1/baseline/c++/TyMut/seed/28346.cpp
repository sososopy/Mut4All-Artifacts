
int outer;
struct A{
  int member = 0;
  auto fn(){
     return [&]{
       outer = member; // invalid
     };
  }
};

