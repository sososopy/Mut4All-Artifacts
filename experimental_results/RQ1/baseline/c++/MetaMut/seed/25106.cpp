

int foo ()
{
  decltype (auto) a = foo;  // OK: decltype (auto) a = &foo;
  return 0; 
}
