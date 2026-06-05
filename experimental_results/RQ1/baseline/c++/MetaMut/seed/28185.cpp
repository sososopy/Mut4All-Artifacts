
  struct X
  {
                          X() : m() {}
      unsigned char       m[ 10 ] ;
  } ;

  int
  main()
  {
      X x ;
      unsigned char const a = x.m[ 0 ] >> 3 ;
  }
