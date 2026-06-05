

void foo (int);

void foo (void)
{
  [&foo]
  {
    foo (0); 
  };
}
