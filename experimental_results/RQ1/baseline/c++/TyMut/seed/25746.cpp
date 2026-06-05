
 struct bug { int a; int *b = [&]{ return &a; }(); };

