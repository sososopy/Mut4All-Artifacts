
// bug_a.ii
module ;
       


namespace STD::RANGES::INNER
{
void Frob ();
}

struct gnu_char_traits
{
  void Frob()
  {
    STD::RANGES::INNER::Frob ();
  }
};


export  module  hello;
export void greeter (gnu_char_traits const &name);

// bug_b.ii
import  hello;

