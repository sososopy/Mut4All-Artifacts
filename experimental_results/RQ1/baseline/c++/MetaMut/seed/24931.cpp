

struct foo
{
  friend void
  bar (void);

  void baz (void)
  {
    bar ();
  }
};

void
bar (void)
{
}

