
constexpr int f ()
{
  char a [] = "ab";
  a [1] = 'c';
  return 0; 
}

constexpr int i = f ();
