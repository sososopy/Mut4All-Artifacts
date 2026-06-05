
int gvar=1;

template<int I>
void tfun(){
  int const var=gvar;
  auto f=[=](){return var*var;};
}

void fun(){
  tfun<1>();
}
