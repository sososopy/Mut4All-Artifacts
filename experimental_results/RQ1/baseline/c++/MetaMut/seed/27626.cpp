constexpr long int
foo ()
{
  return (long int) "foo"
#ifdef FOO
    - 1
#endif
    ;
}

constexpr long int l = foo ();

