
struct bool_convert
{
  explicit operator bool() { return true; }
};

bool x(bool_convert());        //1
bool x1{bool_convert{}};       //2
bool x2 = bool{bool_convert{}}; //3
