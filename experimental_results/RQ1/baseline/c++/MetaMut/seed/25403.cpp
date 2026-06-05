


template < typename T > 
class A 
{   
  static int i;   
};

//okay: template < typename T > int A <T>::i = 100;
template < typename T > int decltype (A < T > ())::i = 100;
