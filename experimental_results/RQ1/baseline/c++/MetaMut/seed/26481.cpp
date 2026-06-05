void funy(int const &){}
void fun(int volatile const &){}
int main(){
  int y ;
  funy(y); // good
  funy(static_cast<int && >(y)); // good
  funy(static_cast<int const && >(y)); // good
  int volatile x ;
  fun(x); // good
  fun(static_cast<int volatile && >(x)); // error
  fun(static_cast<int volatile const && >(x)); // error
}
