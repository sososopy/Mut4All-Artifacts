

template < class T = int > void f (T)
{ 
  auto g = [&a = f] () {};
}

int main ()
{ 
  f (0);
  return 0;
}
