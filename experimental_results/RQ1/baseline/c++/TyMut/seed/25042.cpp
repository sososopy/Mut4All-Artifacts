

int i;

void foo ()
{ 
  int a[i];
  [&] { __typeof (a) x; } ();
}
