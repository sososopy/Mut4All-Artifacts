
template<int I> struct frob
{
  int i;
  void m ()
  {
    auto b = [] {};  // named __closure in the template
    b ();
  }
};

  
frob<0> i;

int x ()
{
  i.m (); // named __this in the instantiation
}
