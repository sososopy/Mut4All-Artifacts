
auto fn( const unsigned char &a,
         const unsigned char &b,
         const unsigned char &c ) {
   return static_cast<unsigned char>(
              static_cast<unsigned char>(
                   a << 1U
              )
              |
              b
          )
          |
          c;
}

