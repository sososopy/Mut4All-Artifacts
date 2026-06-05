

template < typename ... Ts > void f (Ts ... args)
{ 
  [ts (args ...)] {} ();
}

int main ()
{ 
  f (0);
  return 0;
}
