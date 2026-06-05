

template< typename A >
struct X
{
  using this_type = X<A>;
//  typedef X<A> this_type;
  static this_type inst;
};

template< typename A >
X<A> X<A>::inst;

