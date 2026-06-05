

struct S
{ 
  int f (void) { return 0; }

  // should be e.g.: int f (int) { return f() ? : 1; }
  int f (int)  { return f ? : 1; }
};
