
void foo ()
{
  int a[2] = { 1, 2 }; 
  for (struct S { S (int) {} } S : a) 
    ;
}
